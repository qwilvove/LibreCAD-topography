#include "project_settings.h"

ProjectSettings::ProjectSettings()
{
    this->scale = 0.01; // Default scale 1:100
}

double ProjectSettings::getScale()
{
    return this->scale;
}

void ProjectSettings::setScale(double scale)
{
    this->scale = scale;
}
