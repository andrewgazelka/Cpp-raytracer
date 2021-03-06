#pragma once

#include <unordered_map>

enum class Command {
    INVALID, // since this is the default
    CAMERA_POS,
    CAMERA_FWD,
    CAMERA_UP,
    PLANE,
    BOX,
    CAMERA_FOV_HA,
    FILM_RESOLUTION,
    OUTPUT_IMAGE,
    CROP,
    MAX_VERTICES,
    MAX_NORMALS,
    VERTEX,
    NORMAL,
    TRIANGLE,
    NORMAL_TRIANGLE,
    SPHERE,
    BACKGROUND,
    MATERIAL,
    DIRECTIONAL_LIGHT,
    POINT_LIGHT,
    SPOT_LIGHT,
    AMBIENT_LIGHT,
    MAX_DEPTH,
    SAMPLES
};

class CommandGetter {
public:
    Command get(const string &input) {
        return commands[input];
    }

    CommandGetter() = default;

private:
    std::unordered_map<std::string, Command> commands{
            {"camera_pos",        Command::CAMERA_POS},
            {"camera_fwd",        Command::CAMERA_FWD},
            {"camera_up",         Command::CAMERA_UP},
            {"camera_fov_ha",     Command::CAMERA_FOV_HA},
            {"film_resolution",   Command::FILM_RESOLUTION},
            {"output_image",      Command::OUTPUT_IMAGE},
            {"max_vertices",      Command::MAX_VERTICES},
            {"crop",              Command::CROP},
            {"max_normals",       Command::MAX_NORMALS},
            {"vertex",            Command::VERTEX},
            {"normal",            Command::NORMAL},
            {"triangle",          Command::TRIANGLE},
            {"plane",             Command::PLANE},
            {"box",               Command::BOX},
            {"normal_triangle",   Command::NORMAL_TRIANGLE},
            {"sphere",            Command::SPHERE},
            {"background",        Command::BACKGROUND},
            {"material",          Command::MATERIAL},
            {"normal_triangle",   Command::NORMAL_TRIANGLE},
            {"directional_light", Command::DIRECTIONAL_LIGHT},
            {"point_light",       Command::POINT_LIGHT},
            {"spot_light",        Command::SPOT_LIGHT},
            {"ambient_light",     Command::AMBIENT_LIGHT},
            {"max_depth",         Command::MAX_DEPTH},
            {"output_file",       Command::OUTPUT_IMAGE}, // to be backward compatible with hw3
            {"image_resolution",  Command::FILM_RESOLUTION}, // to be backward compatible with hw3
            {"samples",           Command::SAMPLES},
    };
};

CommandGetter Commands = CommandGetter();
