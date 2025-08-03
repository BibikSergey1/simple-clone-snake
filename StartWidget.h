#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QWidget>
class DlgGameUserSettings;
class GameUserSettings;

class StartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StartWidget(QWidget *parent = nullptr);
    ~StartWidget();

    DlgGameUserSettings *getDialogSettings() const;

private slots:
    void onStartGame();
    void onSettingsDialog();

signals:
    void signalStartGame();
    void signalSetGameSettings(int cols, int rows, int countFoods, int delay);

private:
    DlgGameUserSettings *dlgSettings;
    std::unique_ptr<GameUserSettings> _settings;
};

#endif // STARTWIDGET_H
