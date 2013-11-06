#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "collaberativefiltering.h"
#include "person.h"
#include "product.h"

class FBGraph_Parser;
class Post;
class Comment;
class Like;

namespace Ui {
        class MainWindow;
    }

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_setTolkenButton_clicked();

    void on_UpdatePeople_clicked();

    void onPeopleAdded(Person* person);
    void onPeopleUpdated(Person* person);
    void onPostAdded(Post* post);
    void onCommentAdded(Comment* comment);
    void onLikeAdded(Like* like);
    void onNewPageParsing();
    void onDoneParsing();

private:
    Ui::MainWindow *ui;
    CollaberativeFiltering cf;
    FBGraph_Parser* parser;

    bool statusParsing;
    long statusPeopleCount;
    long statusRelevantPeopleCount;
    long statusPagesCount;
    long statusPostsCount;
    long statusRelevantPostsCount;
    long statusCommentsCount;
    long statusRelevantCommentsCount;
    long statusLikesCount;
    long statusRelevantLikesCount;

    void            convertPersonToGroup(Person person, QList< GROUP>& groups);
    void            convertProductStringToProduct(Product product, PRODUCT& productenum );
    void            addPersonWithProducts(Person person, bool train);

    void            setupProductsTree();
    void            updateStatusBar();
};

#endif // MAINWINDOW_H
