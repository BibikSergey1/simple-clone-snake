#ifndef DLGGAMEUSERSETTINGS_H
#define DLGGAMEUSERSETTINGS_H

#include <QDialog>
class GameUserSettings;

namespace Ui {
class DlgGameUserSettings;
}

class DlgGameUserSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DlgGameUserSettings(GameUserSettings *settings, QWidget *parent = nullptr);
    ~DlgGameUserSettings();

    int getRows() const;
    int getCols() const;
    int getCountFoods() const;
    int getDelay() const;

    void setCountFoods(int value);
    void setDelay(int value);

public slots:
    void setCols(int value);
    void setRows(int value);

private:
    Ui::DlgGameUserSettings *ui;
    GameUserSettings *_settings = nullptr;

private slots:
    void applay();
    void cancel();
};

#endif // DLGGAMEUSERSETTINGS_H
