#ifndef TT_CLASS_PROJECT_SETTINGS_H
#define TT_CLASS_PROJECT_SETTINGS_H

class ProjectSettings
{
public:
    ProjectSettings();

    double getScale();

    void setScale(double scale);

private:
    double scale; // Project scale stored as a double (e.g. 0.01 for 1:100)
};

#endif // TT_CLASS_PROJECT_SETTINGS_H
