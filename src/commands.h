#pragma once

#include <unordered_map>


enum class Command {
    INVALID, // since this is the default
    CAMERA_POS,
    CAMERA_FWD,
    CAMERA_UP,
    CAMERA_FOV_HA,
    FILM_RESOLUTION,
    OUTPUT_IMAGE,
    MAX_VERTICES,
    MAX_NORMALS,
    VERTEX,
    NORMAL,
    TRIANGLE,
    NORMAL_TRIANGLE,
    SPHERE,
    BACKGROUND,
    MATERIAL,
    NORMAL_TRIANGLE
};

class CommandGetter {
public:
    Command get(const string &input) {
        return commands[input];
    }

private:
    CommandGetter() = default;

    std::unordered_map<std::string, Command> commands{
            {"camera_pos",      Command::CAMERA_POS},
            {"camera_fwd",      Command::CAMERA_FWD},
            {"camera_up_",      Command::CAMERA_UP},
            {"camera_fov_ha",   Command::CAMERA_FOV_HA},
            {"film_resolution", Command::FILM_RESOLUTION},
            {"output_image",    Command::OUTPUT_IMAGE},
            {"max_vertices",    Command::MAX_VERTICES},
            {"max_normals",     Command::MAX_NORMALS},
            {"vertex",          Command::VERTEX},
            {"normal",          Command::NORMAL},
            {"triangle",        Command::TRIANGLE},
            {"normal_triangle", Command::NORMAL_TRIANGLE},
            {"sphere",          Command::SPHERE},
            {"background",      Command::BACKGROUND},
            {"material",        Command::MATERIAL},
            {"normal_triangle", Command::NORMAL_TRIANGLE},
    };
};

CommandGetter Commands = CommandGetter()


