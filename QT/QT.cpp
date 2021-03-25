#include <iostream>
#include <QApplication>
#include <QListWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QWidget>
#include <QDateTime>
#include <QFile>
#include <QMessageBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QSize>
#include <fstream>

static QListWidget* list;
// TODO Переименовать и дописать согласно варианту
struct Information {
    QString name;
    QString path;

    QFile* f;
};

// TODO Переименовать и дописать согласно варианту
struct InformationItem : QListWidgetItem {
    Information* object;

    InformationItem(Information* object) : QListWidgetItem(object->name), object(object) { }

    InformationItem(const Information&) = delete;

    InformationItem& operator=(const Information&) = delete;

    QString getName() {
        return object->name;
    }

    QString getPath() {
        return object->path;
    }

    QFile* getFile() {
        return object->f;
    }

    ~InformationItem() {
    }
};

static QList<InformationItem*> filesList;

static void addItem() {
    auto file = new Information;

    file->name = "file" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".txt";
    file->path = "C:/Qtrojeects/FilesBucket/file" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".txt";

    QFile qfile(file->path);
    qfile.open(QFile::ReadWrite);
    qfile.close();

    file->f = &qfile;

    auto item = new InformationItem(file);

    filesList.push_back(item);
    list->addItem(item);
}

void showInformation(QListWidgetItem* item) {
    QString data = item->text();
    for (int i = 0; i < filesList.size(); ++i) {
        InformationItem tmp = *filesList.at(i);
        if (data == tmp.getName()) {
            QString path = tmp.getPath();
            QByteArray qba = path.toLocal8Bit();
            const char* fileName = qba.data();

            std::ofstream out;
            out.open(fileName);
            if (out.is_open()) {
                out << "Hello World!" << std::endl;
            }

            /*const char* fileName = qba.data();
            FILE* p = fopen(fileName, "r+");
            if (feof(p)) {
                std::cout << "File is empty" << std::endl;
            }
            else
            {
                std::cout << "QQ" << std::endl;
            }
            break;*/
        }
    }
}

static void saveToFile() {
    if (list->currentItem() == NULL) {
        QMessageBox::warning(NULL, "Information", "Select the item or Add the item");
    }
}

static void removeItem() {
    if (list->count() == NULL) {
        QMessageBox::warning(NULL, "Information", "Select the item or Add the item");

    }
    else
    {
        if (list->currentItem() == NULL) {
            QMessageBox::warning(NULL, "Information", "Select the item or Add the item");
        }
        else {
            QString fileName = "C:/Qtrojeects/FilesBucket/" + list->currentItem()->text();
            QByteArray qba = fileName.toLocal8Bit();

            const char* c_str = qba.data();
            for (InformationItem* i : filesList) {
                if (i->getPath() == fileName) {
                    filesList.removeOne(i);
                }
            }
            remove(c_str);
            qDeleteAll(list->selectedItems());
        }
    }
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Создаём окно
    QWidget window;
    window.setWindowTitle(u8"Object Manager");

    // Создаём список и кнопки добавления/удаления
    list = new QListWidget;
    auto addButton = new QPushButton("Add");
    auto removeButton = new QPushButton("Remove");
    auto saveButton = new QPushButton("Save");

    auto hbl1 = new QHBoxLayout();
    auto hbl2 = new QHBoxLayout();
    auto hbl3 = new QHBoxLayout();
    auto vbl = new QVBoxLayout();

    auto textA = new QLabel(" A:");
    auto inputA = new QSpinBox();

    auto textB = new QLabel(" B:");
    auto inputB = new QSpinBox();

    auto textResult = new QLabel("  Result:");
    auto outputResult = new QLabel();

    hbl1->addWidget(textA);
    hbl1->addWidget(inputA);
    hbl2->addWidget(textB);
    hbl2->addWidget(inputB);
    hbl3->addWidget(textResult);
    hbl3->addWidget(outputResult);

    vbl->addLayout(hbl1);
    vbl->addLayout(hbl2);
    //vbl->addItem(hbl3);

    QObject::connect(list, &QListWidget::itemClicked, showInformation);
    QObject::connect(addButton, &QPushButton::clicked, addItem);
    QObject::connect(removeButton, &QPushButton::clicked, removeItem);
    QObject::connect(saveButton, &QPushButton::clicked, saveToFile);

    // Создаём поля для отображения информации об объекте
    auto groupBox = new QGroupBox(u8"Information about object");
    //auto editLayout = ...;
    //groupBox->setLayout(editLayout);

    QString message;
    // Расположение элементов на форме
    auto layout = new QGridLayout;
    layout->addWidget(list, 0, 0, 1, 2);
    layout->addWidget(addButton, 1, 0);
    layout->addWidget(removeButton, 1, 1);
    layout->addWidget(saveButton, 1, 2);
    layout->addWidget(groupBox, 0, 2, 2, 1);
    layout->addLayout(vbl, 0, 2);

    // Показываем окно
    window.setLayout(layout);
    window.show();

    return QApplication::exec();
}