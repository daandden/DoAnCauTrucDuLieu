#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QLabel>
#include <QVariant>
#include <QList>
#include <QDebug>
#include <QString>
#include <sstream>
#include <string>
#include <QPainter>
#include <QMessageBox>
#include <QPen>
#include <QThread>
#include <QPushButton>
#include <Windows.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , static_position {0}
    , check_painter {false}
{
    ui->setupUi(this);
    connect(ui->pushButton_Exit_2, SIGNAL(clicked()), ui->pushButton_Exit, SLOT(click()));
    ui->stackedWidget->setCurrentWidget(ui->page_Start);
    ui->pushButton_Insert->hide();
    ui->pushButton_Delete->hide();
    ui->pushButton_Search->hide();
    gridLayout = new QGridLayout(ui->widget_Main); //setLayout cho widget_main
}

MainWindow::~MainWindow()
{
    qDeleteAll(listQLabel);
    listQLabel.clear();
    delete gridLayout;
    delete ui;
}
void MainWindow::on_pushButton_Exit_clicked()
{
    QCoreApplication::quit();
}

void MainWindow::on_pushButton_Start_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_Main);
}

void MainWindow::on_pushButton_Create_clicked()
{
    bool ok;
    int size = QInputDialog::getInt(ui->page_Main, "Create Node", "Số node cần tạo: ", 1, 1, 99, 1, &ok);
    if (ok)
    {
        for (int i = 0; i < size; i++)
        {
            bool ok;
            QString str = "Nhập giá trị node thứ " + QVariant(i+1).toString() + ":";
            int data = QInputDialog::getInt(ui->page_Main, "Create Node", str, 0, -999999, 999999, 1, &ok);
            if (ok)
            {
                list.appendNode(data);
                createLabel(i);
            }
            else
            {
                if (i == 0) //trường hợp nhấn cancel ở node đầu tiên
                {
                    goto A;
                }
                break;
            }
        }
        ui->pushButton_Create->hide();
        ui->pushButton_Insert->show();
        ui->pushButton_Delete->show();
        ui->pushButton_Search->show();
        showListQLabel();
        this->check_painter = true;
    A:
        {}
    }
}

void MainWindow::on_pushButton_Insert_clicked()
{
    if (static_position != 0) //chuyen node black color
    {
        listQLabel.operator[](static_position - 1)->setStyleSheet("font-size: 25px;"
                                                                  "border-width: 1px;"
                                                                  "border-style: solid;"
                                                                  "border-radius: 10px;"
                                                                  "padding: 4px;"
                                                                  "color: black;"
                                                                  "border-color: black;");
    }
    QMessageBox msgBox;
    QPushButton *pushButton = msgBox.addButton("Push Node", QMessageBox::ActionRole);
    QPushButton *appenButton = msgBox.addButton("Appen Node", QMessageBox::ActionRole);
    QPushButton *insertButton = msgBox.addButton("Insert Node", QMessageBox::ActionRole);
    msgBox.setDefaultButton(insertButton);
    msgBox.exec();

    ////PUSH NODE////
    if (msgBox.clickedButton() == pushButton)
    {
        bool ok;
        int data = QInputDialog::getInt(ui->page_Main, "Push Node", "Nhập giá trị: ", 0, -999999, 999999, 1, &ok);
        if (ok)
        {
            list.pushNode(data);
            static_position = 1;
            insertLabel(static_position);
            update(); //update lại arrow
            listQLabel.operator[](static_position - 1)->setStyleSheet("font-size: 25px;"
                                                                      "border-width: 1px;"
                                                                      "border-style: solid;"
                                                                      "border-radius: 10px;"
                                                                      "padding: 4px;"
                                                                      "color: blue;"
                                                                      "border-color: blue;");
            qApp->processEvents(); //refresh lại ui
        }
    }

    ///APPEND NODE////
    if (msgBox.clickedButton() == appenButton)
    {
        bool ok;
        int data = QInputDialog::getInt(ui->page_Main, "Append Node", "Nhập giá trị: ", 0, -999999, 999999, 1, &ok);
        if (ok)
        {
            static_position = listQLabel.size() + 1;
            change_nodes_color(static_position);
            list.appendNode(data);
            insertLabel(static_position);
            update(); //update lại arrow
            listQLabel.operator[](static_position - 1)->setStyleSheet("font-size: 25px;"
                                                                      "border-width: 1px;"
                                                                      "border-style: solid;"
                                                                      "border-radius: 10px;"
                                                                      "padding: 4px;"
                                                                      "color: blue;"
                                                                      "border-color: blue;");
            qApp->processEvents(); //refresh lại ui
        }
    }

    ////INSERT NODE////
    if (msgBox.clickedButton() == insertButton)
    {
        bool ok;
        static_position = QInputDialog::getInt(ui->page_Main, "Insert Node", "Nhập vị trí: ", 0, 1, list.getLength() + 1, 1, &ok);
        if (ok)
        {
            bool ok;
            int data = QInputDialog::getInt(ui->page_Main, "Insert Node", "Nhập giá trị: ", 0, -999999, 999999, 1, &ok);
            if (ok)
            {
                change_nodes_color(static_position);
                list.addNode(data, static_position);
                insertLabel(static_position);
                update(); //update lại arrow
                listQLabel.operator[](static_position - 1)->setStyleSheet("font-size: 25px;"
                                                                          "border-width: 1px;"
                                                                          "border-style: solid;"
                                                                          "border-radius: 10px;"
                                                                          "padding: 4px;"
                                                                          "color: blue;"
                                                                          "border-color: blue;");
                /*Sau khi thêm thì sau node đã thêm là node màu đỏ vì cho chạy màu đỏ đến vị trí static_position rồi thêm node */
                ////DOI MAU NODE SAU NODE INSERT TU MAU DO THANH MAU BlACK////
                if (static_position < listQLabel.size())
                {
                    listQLabel.operator[](static_position)->setStyleSheet("font-size: 25px;"
                                                                          "border-width: 1px;"
                                                                          "border-style: solid;"
                                                                          "border-radius: 10px;"
                                                                          "padding: 4px;"
                                                                          "color: black;"
                                                                          "border-color: black;");
                }
                qApp->processEvents(); //refresh lại ui
            }
            else
            {
                static_position = 0; //đưa static_position trở lại vị trí ban đầu vì static_position có thể lớn hơn listQLabel.size()
            }

        }

    }
}

void MainWindow::on_pushButton_Search_clicked()
{
    if (static_position != 0)//chuyen node search tro lai black color
    {
        listQLabel.operator[](static_position - 1)->setStyleSheet("font-size: 25px;"
                                                                  "border-width: 1px;"
                                                                  "border-style: solid;"
                                                                  "border-radius: 10px;"
                                                                  "padding: 4px;"
                                                                  "color: black;"
                                                                  "border-color: black;");
    }
    bool ok;
    int data = QInputDialog::getInt(ui->page_Main, "Search", "Nhập giá trị cần tìm", 0, -999999, 999999, 1, &ok);
    if (ok)
    {
        static_position = list.searchNode(data);
        if (static_position == -1)
        {
            change_nodes_color(listQLabel.size());
            QString str = "Không tìm thấy " + QVariant(data).toString() + " trong danh sách";
            QMessageBox::critical(this, "", str);
            static_position = listQLabel.size(); //node cuối màu đỏ nên để static_positon thành node cuối để đổi màu thành đen
        }
        else
        {
            change_nodes_color(static_position);
            listQLabel.operator[](static_position - 1)->setStyleSheet("font-size: 25px;"
                                                                      "border-width: 1px;"
                                                                      "border-style: solid;"
                                                                      "border-radius: 10px;"
                                                                      "padding: 4px;"
                                                                      "color: blue;"
                                                                      "border-color: blue;");

            QString str = "Tìm thấy " + QVariant(data).toString() + " tại node thứ " + QVariant(static_position).toString();
            QMessageBox::about(this, "", str);
        }

    }
}
void MainWindow::on_pushButton_Delete_clicked()
{
    if (static_position != 0)//chuyen node search tro lai black color
    {
        listQLabel.operator[](static_position - 1)->setStyleSheet("font-size: 25px;"
                                                                  "border-width: 1px;"
                                                                  "border-style: solid;"
                                                                  "border-radius: 10px;"
                                                                  "padding: 4px;"
                                                                  "color: black;"
                                                                  "border-color: black;");
    }
    if (listQLabel.size() == 0)
    {
        QMessageBox::about(this, "", "Danh sách trống");
    }
    else
    {
        QMessageBox msgBox;
        QPushButton *deleteFirstNode = msgBox.addButton("Delete First Node", QMessageBox::ActionRole);
        QPushButton *deleteNode = msgBox.addButton("Delete Given Node", QMessageBox::ActionRole);
        msgBox.setDefaultButton(deleteNode);
        msgBox.exec();
        ////DELETE FIRST NODE////
        if (msgBox.clickedButton() == deleteFirstNode)
        {
            static_position = 1;
            listQLabel.operator[](static_position - 1)->setStyleSheet("font-size: 25px;"
                                                                      "border-width: 1px;"
                                                                      "border-style: solid;"
                                                                      "border-radius: 10px;"
                                                                      "padding: 4px;"
                                                                      "color: blue;"
                                                                      "border-color: blue;");
            qApp->processEvents();
            Sleep(500);
            list.deleteFirstNode();
            deleteLabel(static_position);
            static_position = 0; //delete nên mất luôn ô đó không cần chuyển màu node thành đen
            update(); //update lại arrow
            qApp->processEvents();
        }
        else if (msgBox.clickedButton() == deleteNode)
        {
            bool ok;
            static_position = QInputDialog::getInt(ui->page_Main, "Delete Node", "Nhập vị trí: ", 0, 1, list.getLength(), 1, &ok);
            if (ok)
            {
                change_nodes_color(static_position);
                listQLabel.operator[](static_position - 1)->setStyleSheet("font-size: 25px;"
                                                                          "border-width: 1px;"
                                                                          "border-style: solid;"
                                                                          "border-radius: 10px;"
                                                                          "padding: 4px;"
                                                                          "color: blue;"
                                                                          "border-color: blue;");
                qApp->processEvents();
                Sleep(500);
                list.deleteNode(static_position);
                deleteLabel(static_position);
                static_position = 0; //delete nên mất luôn ô đó không cần chuyển màu node thành đen
                update(); //update lại arrow
                qApp->processEvents();
            }
        }
    }
}


void MainWindow::createLabel(int index)
{
    ////CREATE lABEL SHOW DATA & NODE POINTER////
    QLabel *newLabel = new QLabel(ui->widget_Main);
    newLabel->setFrameStyle(QFrame::WinPanel | QFrame::Plain);
    newLabel->setAlignment(Qt::AlignCenter);
    newLabel->setText(QVariant(list.getNode(index + 1)).toString());
    newLabel->setSizePolicy(QSizePolicy::Maximum , QSizePolicy::Maximum);
    newLabel->setStyleSheet("font-size: 25px;"
                            "border-width: 1px;"
                            "border-style: solid;"
                            "border-radius: 10px;"
                            "padding: 4px;"
                            "color: black;"
                            "border-color: black;");
    listQLabel.append(newLabel);
}

void MainWindow::insertLabel(int position)
{
    QLabel *newLabel = new QLabel(ui->widget_Main);
    newLabel->setFrameStyle(QFrame::WinPanel | QFrame::Plain);
    newLabel->setAlignment(Qt::AlignCenter);
    newLabel->setText(QVariant(list.getNode(position)).toString());
    newLabel->setSizePolicy(QSizePolicy::Maximum , QSizePolicy::Maximum);
    newLabel->setStyleSheet("font-size: 25px;"
                            "border-width: 1px;"
                            "border-style: solid;"
                            "border-radius: 10px;"
                            "padding: 4px;"
                            "color: black;"
                            "border-color: black;");
    listQLabel.insert(position - 1, newLabel);
    showListQLabel();
}

void MainWindow::deleteLabel(int position)
{

    delete listQLabel.at(position - 1);
    listQLabel.removeAt(position - 1 );
    showListQLabel();
}

void MainWindow::showListQLabel()
{
    delete gridLayout;
    gridLayout = new QGridLayout(ui->widget_Main); //setLayout cho widget_main
    //thêm label trong listQLabel vào gridLayout
    //tim số số hàng ngang
    //nếu hàng số lẻ thêm label từ trái qua fai
    //nếu hàng số chẵn thêm label từ phai qua trai
    int row = list.getLength() / 6 + 1;
    for (int i = 0; i < row; i++)
    {
        if (i != row - 1)
        {
            if (i % 2 == 0)
            {
                for (int j = i * 6; j < (i + 1) * 6; j++)
                {
                    gridLayout->addWidget(listQLabel.value(j), i, j - (i * 6), Qt::AlignCenter);
                }
            }
            if (i % 2 == 1)
            {
                for (int j = i * 6; j < (i + 1) * 6; j++)
                {
                    gridLayout->addWidget(listQLabel.value(j), i, 5 - (j -(i * 6)), Qt::AlignCenter);
                }
            }
        }
        if (i == row - 1)
        {
            if (i % 2 == 0)
            {
                for (int j = i * 6; j < list.getLength(); j++)
                {
                    gridLayout->addWidget(listQLabel.value(j), i, j - (i * 6), Qt::AlignCenter);

                }
            }
            if (i % 2 == 1)
            {
                for (int j = i * 6; j < list.getLength(); j++)
                {
                    gridLayout->addWidget(listQLabel.value(j), i,5 - (j -(i * 6)), Qt::AlignCenter);
                }
            }
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *) //ve mui ten
{
    QPainter painter(this);
    QPen pen;
    pen.setWidthF(1.1);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    if (check_painter == true)
    {
        if (list.getLength() == 1)
        {
            pen.setColor(Qt::red);
            painter.setPen(pen);
            painter.drawLine(listQLabel.at(0)->pos() + QPoint(listQLabel.at(0)->width() / 2 + 18,0), listQLabel.at(0)->pos() + QPoint(listQLabel.at(0)->width() / 2 + 18, 18));
            painter.drawLine(listQLabel.at(0)->pos() + QPoint(listQLabel.at(0)->width() / 2 + 13,12), listQLabel.at(0)->pos() + QPoint(listQLabel.at(0)->width() / 2 + 18, 18)); //mũi tên dọc 5 width, 6 height
            painter.drawLine(listQLabel.at(0)->pos() + QPoint(listQLabel.at(0)->width() / 2 + 23,12), listQLabel.at(0)->pos() + QPoint(listQLabel.at(0)->width() / 2 + 18, 18));
            pen.setColor(Qt::red);
            painter.setPen(pen);
        }
        else
        {
            for (int i = 0; i < list.getLength() - 1; i++) // có exception là list.getLength() = 1 thì sẽ k vẽ
            {
                //vẽ mũi tên trên node đầu tiên
                if (i == 0)
                {
                    pen.setColor(Qt::red);
                    painter.setPen(pen);
                    painter.drawLine(listQLabel.at(i)->pos() + QPoint(listQLabel.at(i)->width() / 2 + 18,0), listQLabel.at(i)->pos() + QPoint(listQLabel.at(i)->width() / 2 + 18, 18));
                    painter.drawLine(listQLabel.at(i)->pos() + QPoint(listQLabel.at(i)->width() / 2 + 13,12), listQLabel.at(i)->pos() + QPoint(listQLabel.at(i)->width() / 2 + 18, 18)); //mũi tên dọc 5 width, 6 height
                    painter.drawLine(listQLabel.at(i)->pos() + QPoint(listQLabel.at(i)->width() / 2 + 23,12), listQLabel.at(i)->pos() + QPoint(listQLabel.at(i)->width() / 2 + 18, 18));
                    pen.setColor(Qt::black);
                    painter.setPen(pen);
                }
                //vẽ mũi tên nối các node
                //nêu i == i * 6 + 5 thì vẽ xuống
                //xet r = i / 6
                //neu r chan thi vẽ mũi tên từ trái qua fai
                //nếu r lẻ thì vẽ mũi tên từ fai qua trái
                if (i % 6 == 5)
                {
                    painter.drawLine(listQLabel.at(i)->pos() + QPoint(listQLabel.at(i)->width() / 2 + 18, 57), listQLabel.at(i + 1)->pos() + QPoint(listQLabel.at(i + 1)->width() / 2 + 18, 18));
                    painter.drawLine(listQLabel.at(i + 1)->pos() + QPoint(listQLabel.at(i + 1)->width() / 2 + 13,12), listQLabel.at(i + 1)->pos() + QPoint(listQLabel.at(i + 1)->width() / 2 + 18, 18)); //mũi tên dọc 5 width, 6 height
                    painter.drawLine(listQLabel.at(i + 1)->pos() + QPoint(listQLabel.at(i + 1)->width() / 2 + 23,12), listQLabel.at(i + 1)->pos() + QPoint(listQLabel.at(i + 1)->width() / 2 + 18, 18));
                }
                else
                {
                    int r = i / 6;
                    if (r % 2 == 0)
                    {
                        painter.drawLine(listQLabel.at(i)->pos() + QPoint(listQLabel.at(i)->width() + 18, 37), listQLabel.at(i + 1)->pos() + QPoint(18, 37)); //dung width() vì tùy số chữ số của label mà vị trí khác nhau
                        painter.drawLine(listQLabel.at(i + 1)->pos() + QPoint(12, 32), listQLabel.at(i + 1)->pos() + QPoint(18, 37)); //mũi tên ngang 6 width, 5 height
                        painter.drawLine(listQLabel.at(i + 1)->pos() + QPoint(12, 42), listQLabel.at(i + 1)->pos() + QPoint(18, 37));
                    }
                    if (r % 2 == 1)
                    {
                        painter.drawLine(listQLabel.at(i)->pos() + QPoint(18, 37), listQLabel.at(i + 1)->pos() + QPoint(listQLabel.at(i + 1)->width() + 18, 37)); //dung width() vì tùy số chữ số của label mà vị trí khác nhau
                        painter.drawLine(listQLabel.at(i + 1)->pos() + QPoint(listQLabel.at(i + 1)->width() + 24, 32) , listQLabel.at(i + 1)->pos() + QPoint(listQLabel.at(i + 1)->width() + 18, 37)); //mũi tên ngang 6 width, 5 height
                        painter.drawLine(listQLabel.at(i + 1)->pos() + QPoint(listQLabel.at(i + 1)->width() + 24, 42) , listQLabel.at(i + 1)->pos() + QPoint(listQLabel.at(i + 1)->width() + 18, 37));
                    }
                }

            }
        }

    }
}

void MainWindow::change_nodes_color(int position) //chuyển đen sang đỏ
{
    for (int i = 0; i < position; i++)
    {
        if (i != 0)
        {
            listQLabel.operator[](i - 1)->setStyleSheet("font-size: 25px;"
                                                        "border-width: 1px;"
                                                        "border-style: solid;"
                                                        "border-radius: 10px;"
                                                        "padding: 4px;"
                                                        "color: black;"
                                                        "border-color: black;");
        }
        if (i == listQLabel.size()) //check cho append node: position = listQLabel.size() + 1
        {
            break;
        }
        listQLabel.operator[](i)->setStyleSheet("font-size: 25px;"
                                                "border-width: 1px;"
                                                "border-style: solid;"
                                                "border-radius: 10px;"
                                                "padding: 4px;"
                                                "color: red;"
                                                "border-color: red;");

        qApp->processEvents();
        Sleep(500);
    }
}

