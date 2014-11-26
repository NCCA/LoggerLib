#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <iostream>
MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

}

MainWindow::~MainWindow()
{
  delete ui;
}





void MainWindow::on_action_Open_triggered()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
          tr("Log Files (*.log);;All (*.*)"));

  if (!fileName.isEmpty()) {
      QFile file(fileName);
      if (!file.open(QIODevice::ReadOnly)) {
          QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
          return;
      }
      QTextStream in(&file);
      //in.readAll();
      //ui->textEdit->setTextColor(Qt::red);
      //ui->textEdit->setText(in.readAll());
      ui->textEdit->clear();
       while(!in.atEnd())
       {
          QString line = in.readLine();
          processTextColour(line);
          //ui->textEdit->append(line);
       }

      file.close();
  }
}

/*
    NORMAL"\x1B[0m"  27
    RED  "\x1B[31m"
    GREEN  "\x1B[32m"
    YELLOW "\x1B[33m"
    BLUE  "\x1B[34m"
    MAGENTA "\x1B[35m"
    CYAN "\x1B[36m"
    WHITE "\x1B[37m"
    RESET "\033[0m"

    // regex \x1B[ ->num->m or \033[0m

 */
void MainWindow::setColour(const QString &s)
{
  if(s.indexOf("\x1B[0m")!=-1)
    ui->textEdit->setTextColor(Qt::blue);
  else if(s.indexOf("\x1B[0m")!=-1)
    ui->textEdit->setTextColor(Qt::black);
  else if(s.indexOf("\033[0m")!=-1)
    ui->textEdit->setTextColor(Qt::black);
  else if(s.indexOf("\x1B[33m")!=-1)
    ui->textEdit->setTextColor(Qt::yellow);
  else if(s.indexOf("\x1B[31m")!=-1)
    ui->textEdit->setTextColor(Qt::red);
  else if(s.indexOf("\x1B[32m")!=-1)
    ui->textEdit->setTextColor(Qt::green);
  else if(s.indexOf("\x1B[35m")!=-1)
    ui->textEdit->setTextColor(Qt::magenta);
  else if(s.indexOf("\x1B[36m")!=-1)
    ui->textEdit->setTextColor(Qt::cyan);
  else if(s.indexOf("\x1B[37m")!=-1)
    ui->textEdit->setTextColor(Qt::white);
}


void MainWindow::processTextColour(const QString &_s)
{

  QRegExp escape("\[[0-9;]*m| \033\[[0-9]*m");
  QString s=_s;
  do
  {
  std::cout<<"initial string "<<s.toStdString()<<"\n";
  int index=s.indexOf(escape);
  int length=escape.matchedLength();
  std::cout<<length<<"\n";
  int end=s.indexOf(QRegExp("\[[0-9;]*m| \033\[[0-9]*m"),length);
  std::cout<<"start "<<index+length<<" "<<(end == -1 ?  s.length() : end)<<"\n";
  QString output=s.mid(index+length,end == -1 ?  s.length() : end);
  setColour(s);
  std::cout<<"output string is "<<output.toStdString()<<"\n";
  if(index==-1 or end==-1)
    break;
  QTextCursor cursor( ui->textEdit->textCursor() );
  //ui->textEdit->insertText(output);
  s=s.mid(end+1,s.length());
  cursor.insertText( s );
  cursor.insertText( QString("\n") );
  std::cout<<"new string "<<s.toStdString()<<"\n";
  }  while(s.length() !=0);

}
