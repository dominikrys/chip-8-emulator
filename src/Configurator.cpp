#include "Configurator.h"

#include <algorithm>
#include <charconv>
#include <filesystem>
#include <iostream>

Configurator::Configurator(int &argc, char **argv) {
    programName_ = std::filesystem::path(argv[0]).filename().string();

    for (int i = 1; i < argc; i++) {
        tokens_.emplace_back(argv[i]);
    }

    // Set up map for mapping mode enums to strings
    modeMap_ = {{Mode::CHIP8,  "8"},
                {Mode::CHIP48, "48"},
                {Mode::SCHIP,  "S"},
    };
}

void Configurator::printUsage() {
    Config defaultConfig{};
    std::cerr << std::boolalpha <<
              "Usage: " + programName_ + " --rom <path> [options]                                                \n\n" \
              "A CHIP-8 emulator.                                                                                  \n" \
              "Options:                                                                                            \n" \
              "   --scale <scale factor>  Set the scale factor of the window. The CHIP-8 screen is 64*32 pixels.   \n" \
              "                           Default: " + std::to_string(defaultConfig.videoScale_) + "\n" \
              "   --cpufreq <frequency>   Set the CPU frequency of the emulator.                                   \n" \
              "                           Default: " + std::to_string(defaultConfig.cpuFrequency_) + "\n" \
              "   --mute                  Mute the emulator.                                                       \n" \
              "                           Default: " << defaultConfig.mute_ << "\n" \
              "   --mode ( 8 | 48 | S )   Choose the way opcodes 8XY6, 8XYE, FX55 and FX65 are executed.           \n" \
              "                           8: execute like on the original CHIP-8. Most games won't work properly.  \n" \
              "                           but this can help very old games.                                        \n" \
              "                           48: execute like on the CHIP-48. Most games will work properly.          \n" \
              "                           S: execute like on the SCHIP (without SCHIP opcode support). The majority\n" \
              "                           of games assume that the FX55 and FX65 will work like on the SCHIP.      \n" \
              "                           Default: " + modeToStr(defaultConfig.mode_) + "\n" \
              "   -h, --help              Display this help dialogue.\n";
}

void Configurator::configure(Config &config) {
    if (argExists("--help") || argExists("-h")) {
        printUsage();
        throw std::runtime_error("Help requested");
    }

    if (config.romPath_ = getArgValue("--rom"); config.romPath_.empty()) {
        printUsage();
        throw std::runtime_error("No ROM path provided");
    }

    if (std::string videoScaleStr = getArgValue("--scale"); !videoScaleStr.empty()) {
        auto result = std::from_chars(videoScaleStr.data(), videoScaleStr.data() + videoScaleStr.size(),
                                      config.videoScale_);

        if (static_cast<bool>(result.ec)) {
            std::cerr << "Couldn't convert given scale value to int, using the default instead: " +
                         std::to_string(config.videoScale_);
        }
    }

    if (std::string cpuFreqStr = getArgValue("--cpufreq"); !cpuFreqStr.empty()) {
        auto result = std::from_chars(cpuFreqStr.data(), cpuFreqStr.data() + cpuFreqStr.size(),
                                      config.cpuFrequency_);

        if (static_cast<bool>(result.ec)) {
            std::cerr << "Couldn't convert given CPU frequency value to int, using the default instead: " +
                         std::to_string(config.cpuFrequency_);
        }
    }

    if (argExists("--mute")) {
        config.mute_ = true;
    }

    if (std::string modeStr = getArgValue("--mode"); !modeStr.empty()) {
        config.mode_ = strToMode(modeStr, config.mode_);
    }
}

std::string Configurator::getArgValue(const std::string &option) const {
    std::vector<std::string>::const_iterator it{std::find(tokens_.begin(), tokens_.end(), option)};

    if (it != tokens_.end() && ++it != tokens_.end()) {
        return *it;
    }

    return "";
}

bool Configurator::argExists(const std::string &option) const {
    return std::find(tokens_.begin(), tokens_.end(), option) != tokens_.end();
}

std::string Configurator::modeToStr(Mode mode) {
    if (modeMap_.find(mode) != modeMap_.end()) {
        return modeMap_[mode];
    } else {
        // This will only occur if the modeMap is not updated after a new mode is added
        return "Unknown";
    }
}

Mode Configurator::strToMode(const std::string &str, Mode defaultMode) {
    for (const auto &it : modeMap_) {
        if (it.second == str) {
            return it.first;
        }
    }

    std::cerr << "Specified mode not found, using default instead: " + modeToStr(defaultMode);
    return defaultMode;
}
