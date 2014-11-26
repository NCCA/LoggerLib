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
  if(s.indexOf("\x1B[34m")!=-1)
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
  // two cases we either have a string with no colour info so just print
  // otherwise split and process for the different colours etc
  if( s.indexOf(escape) == -1)
  {
    QTextCursor cursor( ui->textEdit->textCursor() );
    cursor.insertText( s );
    cursor.insertText( QString("\n") );
  }
  else
  {
    QTextCursor cursor( ui->textEdit->textCursor() );
    do
    {
      // first set the colour from the first escape string
      setColour(s);
      //  now we need to find the substring to print
      // it could be the rest of the string
      int newStringStart=s.indexOf(escape);
      int length=escape.matchedLength();

      int start=newStringStart+length;

      if (start <0 )
        start=0;

      int newStringEnd=s.indexOf(escape,start);
      int end=newStringEnd;
      if(end == -1)
        end=s.length();
      QString output=s.mid(start,(end+1)-start);
      cursor.insertText( output );
      s=s.mid(end+1,s.length());
    }while(s.length() !=0);
    cursor.insertText( QString("\n") );
  }
}
