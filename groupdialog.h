#ifndef GROUPDIALOG_H
#define GROUPDIALOG_H

#include <QDialog>

namespace Ui {
class GroupDialog;
}

class GroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GroupDialog(QWidget *parent = nullptr);

    void setNewTags(QStringList tags);

    ~GroupDialog();

private:
    Ui::GroupDialog *ui;
    bool ok = false;

private slots:
    void addTagToList();
    void removeTagFromList();
    void sendTagList();
signals:
    void dialogRes(const std::vector<std::string>& tags);
};

#endif // GROUPDIALOG_H
