#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QList>
#include <QGridLayout>
#include <QVBoxLayout>
#include "list.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow(); 
    void createLabel(int index);
    void insertLabel(int position);
    void deleteLabel(int position);
    void showListQLabel();
    void change_nodes_color(int position); //doi mau node den vi tri position
private slots:
    void on_pushButton_Exit_clicked();

    void on_pushButton_Start_clicked();

    void on_pushButton_Create_clicked();

    void on_pushButton_Insert_clicked();

    void on_pushButton_Delete_clicked();

    void on_pushButton_Search_clicked();
protected:
    void paintEvent(QPaintEvent *event);


private:
    Ui::MainWindow *ui;
    List list;
    QList<QLabel*> listQLabel;
    QGridLayout *gridLayout; //tao gridLayout cho widget_Main
    int static_position; // vi tri cua node duoc search
    bool check_painter; //de khong hien arrow luc tao list
};
#endif // MAINWINDOW_H
