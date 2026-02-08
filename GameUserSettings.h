#pragma once
#include <vector>

class GameUserSettings
{
public:
    GameUserSettings();
    ~GameUserSettings();

    std::vector<int> settings;

    bool writeSettings();
    bool readSettings();
};
