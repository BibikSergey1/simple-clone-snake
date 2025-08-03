#include "GameUserSettings.h"
#include <fstream>
#include <iostream>
#include <QDebug>

GameUserSettings::GameUserSettings()
{
    settings.push_back(10);
    settings.push_back(10);
    settings.push_back(5);
    settings.push_back(450);
}

GameUserSettings::~GameUserSettings() {}

bool GameUserSettings::writeSettings()
{
    std::ofstream fsOut("Settings.dat", std::ios_base::out | std::ios_base::binary);
    if (fsOut.is_open())
    {
        for (size_t ii = 0; ii < settings.size(); ++ii)
        {
            fsOut << settings[ii] << "\n";
        }
        fsOut.close();
    }
    else
    {
        return false;
    }

    return true;
}

bool GameUserSettings::readSettings()
{
    std::ifstream fsIn("Settings.dat", std::ios_base::in | std::ios_base::binary);
    if(fsIn.is_open())
    {
        for (size_t ii = 0; ii < settings.size(); ++ii)
        {
            fsIn >> settings[ii];
        }
        fsIn.close();
    }
    else
    {
        return false;
    }

    return true;
}
