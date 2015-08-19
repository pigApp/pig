#ifndef UI_H
#define UI_H

#include "player.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QPixmap>
#include <QMessageBox>
#include <QAbstractItemView>
#include <QScrollArea>
#include <QScrollBar>
#include <QButtonGroup>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDesktopWidget>//
#include <QDesktopServices>
#include <QClipboard>
#include <QCompleter>
#include <QToolTip>

#include <QFont>

QT_BEGIN_NAMESPACE

class Ui_PIG
{
public:
    QPushButton *b_back;
    QPushButton *b_minimize;
    QPushButton *b_close;
    QVBoxLayout *main_layout;

    void setupUi(QWidget *PIG)
    {
        QFont f(":/font-global");
        f.setPointSize(16);//TODO: PORCENTAJE.
        f.setCapitalization(QFont::AllUppercase);
        f.setBold(true);

        QFont f_toolTip(":/font-global");
        f_toolTip.setPointSize(12);//TODO: PORCENTAJE.
        f_toolTip.setCapitalization(QFont::AllUppercase);
        f_toolTip.setBold(true);

        QBrush br_base(QColor(10, 10, 10, 255));
        QBrush br_white(QColor(255, 255, 255, 255));
        QBrush br_black(QColor(0, 0, 0, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Base, br_base);
        p.setBrush(QPalette::Active, QPalette::Window, br_base);
        p.setBrush(QPalette::Active, QPalette::WindowText, br_white);
        p.setBrush(QPalette::Active, QPalette::Text, br_white);
        p.setBrush(QPalette::Active, QPalette::Highlight, br_black);
        p.setBrush(QPalette::Active, QPalette::Button, br_base);
        p.setBrush(QPalette::Active, QPalette::ButtonText, br_white);
        p.setBrush(QPalette::Disabled, QPalette::Base, br_base);
        p.setBrush(QPalette::Disabled, QPalette::Window, br_base);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Text, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, br_base);
        p.setBrush(QPalette::Disabled, QPalette::Button, br_base);
        p.setBrush(QPalette::Disabled, QPalette::ButtonText, br_black);

        QPalette p_toolTip = QToolTip::palette();
        p_toolTip.setBrush(QPalette::All, QPalette::ToolTipBase, br_white);
        p_toolTip.setBrush(QPalette::All, QPalette::ToolTipText, br_black);

        QToolTip::setFont(f_toolTip);
        QToolTip::setPalette(p_toolTip);

        b_back = new QPushButton(QIcon(":/icon-back"), NULL, PIG);
        b_back->setGeometry(1865, 5, 8, 16);//TODO: PORCENTAJE.
        b_back->setToolTip("BACK");
        b_back->setFlat(true);
        b_back->setParent(PIG);
        b_back->setHidden(true);

        b_minimize = new QPushButton(QIcon(":/icon-minimize"), NULL, PIG);
        b_minimize->setGeometry(1880, 5, 16, 16);//TODO: PORCENTAJE.
        b_minimize->setMouseTracking(true);
        b_minimize->setFlat(true);
        b_minimize->setParent(PIG);

        b_close = new QPushButton(QIcon(":/icon-close"), NULL, PIG);
        b_close->setGeometry(1900, 5, 16, 16);//TODO: PORCENTAJE.
        b_close->setMouseTracking(true);
        b_close->setFlat(true);
        b_close->setParent(PIG);

        main_layout = new QVBoxLayout(PIG);

        PIG->setFont(f);
        PIG->setPalette(p);
        PIG->setLayout(main_layout);
    }
};

class Ui_Authotization
{
public:
    QPalette p;
    QPalette p_error;
    QLabel *lb;
    QLineEdit *input;
    QPushButton *b_reset;
    QHBoxLayout *l_input;
    QVBoxLayout *l;

    void setupUi(bool set, bool setted, QWidget *Authorization)
    {
        QBrush br_base(QColor(10, 10, 10, 255));
        QBrush br_alternate_base(QColor(15, 15, 15, 255));
        QBrush br_white(QColor(255, 255, 255, 255));
        QBrush br_black(QColor(0, 0, 0, 255));
        QBrush br_dark(QColor(40, 40, 40, 255));
        QBrush br_red(QColor(115, 10, 10, 255));

        p.setBrush(QPalette::Active, QPalette::Base, br_alternate_base);
        p.setBrush(QPalette::Active, QPalette::Window, br_alternate_base);
        p.setBrush(QPalette::Active, QPalette::WindowText, br_white);
        p.setBrush(QPalette::Active, QPalette::Button, br_base);
        p.setBrush(QPalette::Active, QPalette::ButtonText, br_white);
        p.setBrush(QPalette::Disabled, QPalette::Base, br_dark);
        p.setBrush(QPalette::Disabled, QPalette::Window, br_dark);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Button, br_base);
        p.setBrush(QPalette::Disabled, QPalette::ButtonText, br_dark);

        p_error.setBrush(QPalette::Active, QPalette::Base, br_red);
        p_error.setBrush(QPalette::Active, QPalette::Window, br_red);
        p_error.setBrush(QPalette::Active, QPalette::Text, br_white);
        p_error.setBrush(QPalette::Active, QPalette::Highlight, br_red);
        p_error.setBrush(QPalette::Disabled, QPalette::Base, br_red);
        p_error.setBrush(QPalette::Disabled, QPalette::Window, br_red);
        p_error.setBrush(QPalette::Disabled, QPalette::Highlight, br_red);

        QString str;
        if (set)
            str = "SET PASSWORD";
        else
            str = "ENTER PASSWORD";

        lb = new QLabel(str, Authorization);
        lb->setPalette(p);
        if (!set)
            lb->setAlignment(Qt::AlignHCenter);

        input = new QLineEdit(Authorization);
        input->setPalette(p);
        input->setEchoMode(QLineEdit::Password);
        if (!set)
            input->setFocus();
        input->setDisabled(set && setted);
        input->setAlignment(Qt::AlignCenter);

        b_reset = new QPushButton(Authorization);
        if  (setted)
            b_reset->setIcon(QIcon(":/icon-reset"));
        else
            b_reset->setIcon(QIcon(":/icon-reset-dark"));
        b_reset->setToolTip("RESET PASSWORD");
        b_reset->setFlat(true);
        b_reset->setDisabled(!setted);
        b_reset->setHidden(!set);

        l_input = new QHBoxLayout;
        l_input->setSpacing(20);//TODO: PORCENTAJE.
        if (set)
            l_input->addWidget(lb);
        l_input->addWidget(input);
        l_input->addWidget(b_reset);

        l = new QVBoxLayout(Authorization);
        if (!set)
            l->addWidget(lb);
        l->addLayout(l_input);
        l->setAlignment(Qt::AlignCenter);

        Authorization->setLayout(l);
    }
};

class Ui_Update
{
public:
    QLabel *lb;
    QPushButton *b_1;
    QPushButton *b_2;
    QHBoxLayout *l;

    void setupUi(QWidget *Update)
    {
        lb = new QLabel("UPDATE AVAILABLE", Update);

        b_1 = new QPushButton(QIcon(":/icon-install"), NULL, Update);
        b_1->setToolTip("ACCEPT UPDATE");
        b_1->setFlat(true);

        b_2 = new QPushButton(QIcon(":/icon-cancel"), NULL, Update);
        b_2->setToolTip("CANCEL UPDATE");
        b_2->setFlat(true);

        l = new QHBoxLayout(Update);
        l->setAlignment(Qt::AlignHCenter);
        l->addWidget(lb);
        l->addSpacing(10);//TODO: PORCENTAJE
        l->addWidget(b_1);
        l->addWidget(b_2);

        Update->setLayout(l);
    }
};

class Ui_Topbar
{
public:
    QPushButton *b_setup;
    QGridLayout *l;

    void setupUi(QWidget *Topbar)
    {
        b_setup = new QPushButton(QIcon(":/icon-setup"), NULL, Topbar);
        b_setup->setToolTip("SETUP");
        b_setup->setFlat(true);
        b_setup->setDisabled(true);

        l = new QGridLayout(Topbar);
        l->setContentsMargins(0, 0, 46, 0);//TODO: PORCENTAJE.
        l->setSpacing(0);
        l->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        l->addWidget(b_setup, 0, 1);

        Topbar->setLayout(l);
    }
};

class Ui_Finder
{
public:
    QGridLayout *_l_topbar;

    QPalette p_filter;
    QPalette p_filter_active;
    QLineEdit *input;
    QButtonGroup *bg_quality;
    QButtonGroup *bg_categories;
    QPushButton *b_filters;
    QPushButton *b_quality_all;
    QPushButton *b_quality_720p;
    QPushButton *b_quality_1080p;
    QPushButton *b_fullMovie;
    QHBoxLayout *l_quality_fullmovie;
    QHBoxLayout *l_filters;
    QGridLayout *l;

    QVector<QPushButton*> b_vector_categories;

    void setupUi(QStringList *movies, QGridLayout *l_topbar, QWidget *Finder)
    {
        _l_topbar = l_topbar;

        QFont f(":/font-global");
        f.setPointSize(24);//TODO: PORCENTAJE.
        f.setCapitalization(QFont::AllUppercase);
        f.setBold(true);

        QFont f_completer(":/font-global");
        f_completer.setPointSize(16);//TODO: PORCENTAJE.
        f_completer.setCapitalization(QFont::AllUppercase);
        f_completer.setBold(true);

        QBrush br_base(QColor(10, 10, 10, 255));
        QBrush br_alternate_base(QColor(15, 15, 15, 255));
        QBrush br_white(QColor(255, 255, 255, 255));
        QBrush br_black(QColor(0, 0, 0, 255));
        QBrush br_dark(QColor(40, 40, 40, 255));
        QBrush br_filter(QColor(30, 30, 30, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Base, br_alternate_base);
        p.setBrush(QPalette::Active, QPalette::Window, br_alternate_base);
        p.setBrush(QPalette::Active, QPalette::WindowText, br_white);
        p.setBrush(QPalette::Active, QPalette::Text, br_white);
        p.setBrush(QPalette::Active, QPalette::Highlight, br_black);
        p.setBrush(QPalette::Active, QPalette::Button, br_base);
        p.setBrush(QPalette::Active, QPalette::ButtonText, br_white);
        p.setBrush(QPalette::Disabled, QPalette::Base, br_alternate_base);
        p.setBrush(QPalette::Disabled, QPalette::Window, br_alternate_base);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Text, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, br_alternate_base);
        p.setBrush(QPalette::Disabled, QPalette::Button, br_base);
        p.setBrush(QPalette::Disabled, QPalette::ButtonText, br_black);

        QPalette p_completer;
        p_completer.setBrush(QPalette::Active, QPalette::Base, br_base);
        p_completer.setBrush(QPalette::Active, QPalette::Window, br_base);
        p_completer.setBrush(QPalette::Active, QPalette::Text, br_white);
        p_completer.setBrush(QPalette::Active, QPalette::Highlight, br_alternate_base);

        p_filter.setBrush(QPalette::Active, QPalette::Button, br_filter);
        p_filter.setBrush(QPalette::Active, QPalette::ButtonText, br_white);
        p_filter.setBrush(QPalette::Disabled, QPalette::Button, br_base);
        p_filter.setBrush(QPalette::Disabled, QPalette::ButtonText, br_black);

        p_filter_active.setBrush(QPalette::Active, QPalette::Button, br_dark);
        p_filter_active.setBrush(QPalette::Active, QPalette::ButtonText, br_white);
        p_filter_active.setBrush(QPalette::Disabled, QPalette::Button, br_base);
        p_filter_active.setBrush(QPalette::Disabled, QPalette::ButtonText, br_black);

        QCompleter *completer = new QCompleter(*movies, Finder);
        completer->popup()->setFont(f_completer);
        completer->popup()->setPalette(p_completer);
        completer->setMaxVisibleItems(10);
        completer->setFilterMode(Qt::MatchContains);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setCompletionMode(QCompleter::PopupCompletion);

        input = new QLineEdit(Finder);
        input->setFont(f);
        input->setPalette(p);
        input->setMaxLength(45);
        input->setCompleter(completer);
        input->setAlignment(Qt::AlignCenter);

        b_filters = new QPushButton(QIcon(":/icon-filters"), NULL, Finder);
        b_filters->setGeometry(15, 11, 24, 24);//TODO: PORCENTAJE.
        b_filters->setCursor(Qt::ArrowCursor);
        b_filters->setFlat(true);
        b_filters->setParent(input);

        l = new QGridLayout;
        l->setContentsMargins(87, 0, 8, 0);//TODO: PORCENTAJE.
        l->setSpacing(0);
        l->addWidget(input, 0, 0);

        _l_topbar->addLayout(l, 0, 0);

        input->setFocus();
    }

    void setupFilterUi(QWidget *Finder)
    {
        QFont f(":/font-global");
        f.setPointSize(12);//TODO: PORCENTAJE.
        f.setCapitalization(QFont::AllUppercase);
        f.setBold(true);

        bg_quality = new QButtonGroup(Finder);

        b_quality_all = new QPushButton("ALL", Finder);
        b_quality_all->setFont(f);
        b_quality_all->setPalette(p_filter_active);
        b_quality_all->setFlat(true);
        b_quality_all->setCheckable(true);
        b_quality_all->setChecked(true);
        b_quality_all->setAutoFillBackground(true);

        b_quality_720p = new QPushButton("720p", Finder);
        b_quality_720p->setFont(f);
        b_quality_720p->setPalette(p_filter);
        b_quality_720p->setFlat(true);
        b_quality_720p->setCheckable(true);
        b_quality_720p->setChecked(false);
        b_quality_720p->setAutoFillBackground(true);

        b_quality_1080p = new QPushButton("1080p", Finder);
        b_quality_1080p->setFont(f);
        b_quality_1080p->setPalette(p_filter);
        b_quality_1080p->setFlat(true);
        b_quality_1080p->setCheckable(true);
        b_quality_1080p->setChecked(false);
        b_quality_1080p->setAutoFillBackground(true);

        bg_quality->addButton(b_quality_all);
        bg_quality->addButton(b_quality_720p);
        bg_quality->addButton(b_quality_1080p);

        b_fullMovie = new QPushButton(" FULLMOVIE ", Finder);
        b_fullMovie->setFont(f);
        b_fullMovie->setPalette(p_filter);
        b_fullMovie->setFlat(true);
        b_fullMovie->setCheckable(true);
        b_fullMovie->setChecked(false);
        b_fullMovie->setAutoFillBackground(true);

        bg_categories = new QButtonGroup(Finder);

        l_quality_fullmovie = new QHBoxLayout;;
        l_quality_fullmovie->addWidget(b_quality_all);
        l_quality_fullmovie->addWidget(b_quality_720p);
        l_quality_fullmovie->addWidget(b_quality_1080p);
        l_quality_fullmovie->addSpacing(10);//TODO: PORCENTAJE.
        l_quality_fullmovie->addWidget(b_fullMovie);
        l_quality_fullmovie->addSpacing(10);//TODO: PORCENTAJE.

        l_filters = new QHBoxLayout;
        l_filters->setContentsMargins(88, 15, 10, 0);//TODO: PORCENTAJE.
        l_filters->addLayout(l_quality_fullmovie);

        _l_topbar->addLayout(l_filters, 1, 0);
    }
};

class Ui_Setup
{
public:
    QPalette p_torrent;
    QPalette p_torrent_edit;
    QLabel *lb_data_binary;
    QLabel *lb_data_database;
    QLabel *lb_data_license;
    QLabel *lb_torrent_ports;
    QLabel *lb_provide_torrent;
    QLabel *lb_provide_code;
    QLabel *lb_provide_bug;
    QLabel *lb_provide_donate;
    QLabel *lb_provide_wallet;
    QLineEdit *input_torrent_port_1;
    QLineEdit *input_torrent_port_2;
    QRadioButton *r_folder_covers;
    QRadioButton *r_folder_torrents;
    QRadioButton *r_folder_movies;
    QPushButton *b_folder_covers_reset;
    QPushButton *b_folder_torrents_reset;
    QPushButton *b_folder_movies_reset;
    QPushButton *b_torrent_ports_reset;
    QPushButton *b_provide_torrent;
    QPushButton *b_provide_code;
    QPushButton *b_provide_bug;
    QPushButton *b_provide_donate;
    QPushButton *b_provide_copy_wallet;
    QGroupBox *g_data;
    QGroupBox *g_authorization;
    QGroupBox *g_folder;
    QGroupBox *g_torrent;
    QGroupBox *g_provide;
    QHBoxLayout *l_data;
    QHBoxLayout *l_authorization;
    QHBoxLayout *l_folder_covers;
    QHBoxLayout *l_folder_torrents;
    QHBoxLayout *l_folder_movies;
    QVBoxLayout *l_folder;
    QHBoxLayout *l_torrent;
    QHBoxLayout *l_provide_torrent;
    QHBoxLayout *l_provide_code;
    QHBoxLayout *l_provide_bug;
    QHBoxLayout *l_provide_donate;
    QVBoxLayout *l_provide;
    QGridLayout *l;

    QClipboard *clipboard;

    void setupUi(QStringList *data, bool **keep_covers, bool **keep_torrents,
                 bool **keep_movies, int **torrent_port_1, int **torrent_port_2,
                 QWidget *authorization, QWidget *Setup)
    {
        QBrush br_base(QColor(10, 10, 10, 255));
        QBrush br_alternate_base(QColor(15, 15, 15, 255));
        QBrush br_white(QColor(255, 255, 255, 255));
        QBrush br_black(QColor(0, 0, 0, 255));
        QBrush br_dark(QColor(40, 40, 40, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Base, br_base);
        p.setBrush(QPalette::Active, QPalette::Window, br_base);
        p.setBrush(QPalette::Active, QPalette::WindowText, br_white);
        p.setBrush(QPalette::Active, QPalette::Text, br_white);
        p.setBrush(QPalette::Active, QPalette::Highlight, br_black);
        p.setBrush(QPalette::Active, QPalette::Button, br_base);
        p.setBrush(QPalette::Active, QPalette::ButtonText, br_white);
        p.setBrush(QPalette::Disabled, QPalette::Base, br_base);
        p.setBrush(QPalette::Disabled, QPalette::Window, br_base);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Text, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, br_base);
        p.setBrush(QPalette::Disabled, QPalette::Button, br_base);
        p.setBrush(QPalette::Disabled, QPalette::ButtonText, br_dark);

        QPalette p_dark;
        p_dark.setBrush(QPalette::Active, QPalette::WindowText, br_dark);

        p_torrent.setBrush(QPalette::Active, QPalette::Base, br_dark);
        p_torrent.setBrush(QPalette::Active, QPalette::Window, br_dark);
        p_torrent.setBrush(QPalette::Active, QPalette::Text, br_black);
        p_torrent.setBrush(QPalette::Active, QPalette::Button, br_base);
        p_torrent.setBrush(QPalette::Active, QPalette::ButtonText, br_white);
        p_torrent.setBrush(QPalette::Disabled, QPalette::Button, br_base);
        p_torrent.setBrush(QPalette::Disabled, QPalette::Window, br_base);
        p_torrent.setBrush(QPalette::Disabled, QPalette::ButtonText, br_dark);

        p_torrent_edit.setBrush(QPalette::Active, QPalette::Base, br_alternate_base);
        p_torrent_edit.setBrush(QPalette::Active, QPalette::Text, br_white);

        g_data = new QGroupBox(Setup);
        g_data->setFlat(true);

        lb_data_binary = new QLabel("PIG "+(*data)[0]+"."+(*data)[1], g_data);
        lb_data_binary->setPalette(p);

        lb_data_database = new QLabel("DATABASE "+(*data)[2], g_data);
        lb_data_database->setPalette(p);

        lb_data_license = new QLabel("GPL 2.0", g_data);
        lb_data_license->setPalette(p);

        l_data = new QHBoxLayout(g_data);
        l_data->setSpacing(20);//TODO: PORCENTAJE.
        l_data->addWidget(lb_data_binary);
        l_data->addWidget(lb_data_database);
        l_data->addWidget(lb_data_license);
        l_data->addStretch();

        g_authorization = new QGroupBox(Setup);
        g_authorization->setFlat(true);

        l_authorization = new QHBoxLayout(g_authorization);
        l_authorization->setContentsMargins(0, 0, 0, 0);
        l_authorization->addWidget(authorization);

        g_folder = new QGroupBox(Setup);
        g_folder->setFlat(true);

        r_folder_covers = new QRadioButton("KEEP LOCAL COPY OF COVERS", g_folder);
        r_folder_covers->setPalette(p);
        r_folder_covers->setChecked(**keep_covers);
        r_folder_covers->setAutoExclusive(false);

        b_folder_covers_reset = new QPushButton(QIcon(":/icon-reset"), NULL, g_folder);
        b_folder_covers_reset->setToolTip("RESET FOLDER");
        b_folder_covers_reset->setFlat(true);

        r_folder_torrents = new QRadioButton("KEEP LOCAL COPY OF TORRENTS", g_folder);
        r_folder_torrents->setPalette(p);
        r_folder_torrents->setChecked(**keep_torrents);
        r_folder_torrents->setAutoExclusive(false);

        b_folder_torrents_reset = new QPushButton(QIcon(":/icon-reset"), NULL, g_folder);
        b_folder_torrents_reset->setToolTip("RESET FOLDER");
        b_folder_torrents_reset->setFlat(true);

        r_folder_movies = new QRadioButton("KEEP LOCAL COPY OF MOVIES", g_folder);
        r_folder_movies->setPalette(p);
        r_folder_movies->setChecked(**keep_movies);
        r_folder_movies->setAutoExclusive(false);

        b_folder_movies_reset = new QPushButton(QIcon(":/icon-reset"), NULL, g_folder);
        b_folder_movies_reset->setToolTip("RESET FOLDER");
        b_folder_movies_reset->setFlat(true);

        l_folder_covers = new QHBoxLayout;
        l_folder_covers->addWidget(r_folder_covers);
        l_folder_covers->insertStretch(1);
        l_folder_covers->addWidget(b_folder_covers_reset);

        l_folder_torrents = new QHBoxLayout;
        l_folder_torrents->addWidget(r_folder_torrents);
        l_folder_torrents->insertStretch(1);
        l_folder_torrents->addWidget(b_folder_torrents_reset);

        l_folder_movies = new QHBoxLayout;
        l_folder_movies->addWidget(r_folder_movies);
        l_folder_movies->insertStretch(1);
        l_folder_movies->addWidget(b_folder_movies_reset);

        l_folder = new QVBoxLayout(g_folder);
        l_folder->addLayout(l_folder_covers);
        l_folder->addLayout(l_folder_torrents);
        l_folder->addLayout(l_folder_movies);

        g_torrent = new QGroupBox(Setup);
        g_torrent->setFlat(true);

        lb_torrent_ports = new QLabel("TORRENT PORTS", g_torrent);
        lb_torrent_ports->setPalette(p);

        QValidator *validPort = new QIntValidator(0, 65000, Setup);

        input_torrent_port_1 = new QLineEdit(QString::number(**torrent_port_1), g_torrent);
        input_torrent_port_1->setPalette(p_torrent);
        input_torrent_port_1->setValidator(validPort);
        input_torrent_port_1->setAlignment(Qt::AlignCenter);

        input_torrent_port_2 = new QLineEdit(QString::number(**torrent_port_2), g_torrent);
        input_torrent_port_2->setPalette(p_torrent);
        input_torrent_port_2->setValidator(validPort);
        input_torrent_port_2->setAlignment(Qt::AlignCenter);

        b_torrent_ports_reset = new QPushButton(g_torrent);
        if ((**torrent_port_1 == 6900) && (**torrent_port_2 == 6999))
            b_torrent_ports_reset->setIcon(QIcon(":/icon-reset-dark"));
        else
            b_torrent_ports_reset->setIcon(QIcon(":/icon-reset"));
        b_torrent_ports_reset->setToolTip("RESET PORTS");
        b_torrent_ports_reset->setFlat(true);
        if ((**torrent_port_1 == 6900) && (**torrent_port_2 == 6999))
            b_torrent_ports_reset->setDisabled(true);

        l_torrent = new QHBoxLayout(g_torrent);
        l_torrent->setSpacing(20);//TODO: PORCENTAJE.
        l_torrent->addWidget(lb_torrent_ports);
        l_torrent->addWidget(input_torrent_port_1);
        l_torrent->addWidget(input_torrent_port_2);
        l_torrent->addWidget(b_torrent_ports_reset);

        g_provide = new QGroupBox(Setup);
        g_provide->setFlat(true);

        lb_provide_torrent = new QLabel("PROVIDE TORRENT", g_provide);
        lb_provide_torrent->setPalette(p);

        b_provide_torrent = new QPushButton(QIcon(":/icon-upload"), NULL, g_provide);
        b_provide_torrent->setToolTip("UPLOAD");
        b_provide_torrent->setFlat(true);

        l_provide_torrent = new QHBoxLayout;
        l_provide_torrent->addWidget(lb_provide_torrent);
        l_provide_torrent->insertStretch(1);
        l_provide_torrent->addWidget(b_provide_torrent);

        lb_provide_code = new QLabel("PROVIDE CODE", g_provide);
        lb_provide_code->setPalette(p);

        b_provide_code = new QPushButton(QIcon(":/icon-upload"), NULL, g_provide);
        b_provide_code->setToolTip("UPLOAD");
        b_provide_code->setFlat(true);

        l_provide_code = new QHBoxLayout;
        l_provide_code->addWidget(lb_provide_code);
        l_provide_code->insertStretch(1);
        l_provide_code->addWidget(b_provide_code);

        lb_provide_bug = new QLabel("REPORT BUG", g_provide);
        lb_provide_bug->setPalette(p);

        b_provide_bug = new QPushButton(QIcon(":/icon-upload"), NULL, g_provide);
        b_provide_bug->setToolTip("REPORT");
        b_provide_bug->setFlat(true);;

        l_provide_bug = new QHBoxLayout;
        l_provide_bug->addWidget(lb_provide_bug);
        l_provide_bug->insertStretch(1);
        l_provide_bug->addWidget(b_provide_bug);

        lb_provide_donate = new QLabel("DONATE", g_provide);
        lb_provide_donate->setPalette(p);

        lb_provide_wallet = new QLabel("ACDF723JKD8239OEWMBN7692BH23489BHJK", g_provide);
        lb_provide_wallet->setPalette(p_dark);
        lb_provide_wallet->setTextInteractionFlags(Qt::TextSelectableByMouse);
        lb_provide_wallet->setHidden(true);

        b_provide_donate = new QPushButton(QIcon(":/icon-more"), NULL, g_provide);
        b_provide_donate->setToolTip("BITCOIN");
        b_provide_donate->setFlat(true);

        b_provide_copy_wallet= new QPushButton(QIcon(":/icon-copy"), NULL, g_provide);
        b_provide_copy_wallet->setToolTip("COPY WALLET");
        b_provide_copy_wallet->setFlat(true);
        b_provide_copy_wallet->setHidden(true);

        l_provide_donate = new QHBoxLayout;
        l_provide_donate->addWidget(lb_provide_donate);
        l_provide_donate->addSpacing(10);
        l_provide_donate->addWidget(lb_provide_wallet);
        l_provide_donate->addSpacing(10);
        l_provide_donate->addWidget(b_provide_copy_wallet);
        l_provide_donate->insertStretch(5);
        l_provide_donate->addWidget(b_provide_donate);

        l_provide = new QVBoxLayout(g_provide);
        l_provide->addLayout(l_provide_torrent);
        l_provide->addLayout(l_provide_code);
        l_provide->addLayout(l_provide_bug);
        l_provide->addLayout(l_provide_donate);

        l = new QGridLayout(Setup);
        l->setContentsMargins(80, 11, 80, 12);//TODO: PORCENTAJE.
        l->setSpacing(0);
        l->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        l->addWidget(g_data, 0, 0);
        l->addWidget(g_authorization, 1, 0);
        l->addWidget(g_folder, 2, 0);
        l->addWidget(g_torrent, 3, 0);
        l->addWidget(g_provide, 4, 0);

        Setup->setLayout(l);
    }
};

class Ui_View
{
public:
    Player *player;

    QLabel *lb_metadata[5];
    QLabel *cover;
    QLabel *backCover;
    QPushButton *b_clear;
    QScrollArea *sa_covers;
    QGroupBox *g_covers;
    QGroupBox *g_info;
    QGridLayout *l_covers;
    QVBoxLayout *l_metadata;
    QGridLayout *l_scenes;
    QHBoxLayout *l_multimedia;
    QVBoxLayout *l_info;
    QVBoxLayout *l;

    QVector<QPushButton*> b_vector_covers;
    QVector<QPushButton*> b_vector_scenes;
    QVector<QLabel*> lb_vector_scenes;

    void setupUi(QWidget *View)
    {
        sa_covers = new QScrollArea(View);
        sa_covers->setFrameStyle(QFrame::NoFrame);
        sa_covers->setWidgetResizable(true);
        sa_covers->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        sa_covers->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        g_covers = new QGroupBox(sa_covers);
        g_covers->setStyleSheet("QGroupBox{ background-color: rgba(10, 10, 10, 255);"
                                    "border: 0; margin: 0; }"
                                    "QGroupBox::title { border: 0; margin: 0; }");
        g_covers->setFlat(true);

        l_covers = new QGridLayout(g_covers);
        l_covers->setContentsMargins(75, 0, 0, 20);//TODO: PORCENTAJE.
        l_covers->setHorizontalSpacing(0);
        l_covers->setVerticalSpacing(5);//TODO: PORCENTAJE.
        l_covers->setAlignment(Qt::AlignLeft | Qt::AlignTop);

        g_covers->setLayout(l_covers);

        sa_covers->setWidget(g_covers);

        l = new QVBoxLayout(View);
        l->addWidget(sa_covers);

        View->setLayout(l);

        //        b_clear = new QPushButton("x", g_covers);
        //        b_clear->setFont(f);
        //        b_clear->setPalette(p);
        //        b_clear->setFlat(true);
        //l_covers->addWidget(b_clear);
    }

    void setupInfoUi(const int &index, const QString &path, const QStringList **data, QWidget *View)
    {
        const QStringList **_data = data;

        QFont f(":/font-global");
        f.setPointSize(24);//TODO: PORCENTAJE.
        f.setCapitalization(QFont::AllUppercase);
        f.setBold(true);

        QFont f_scenes(":/font-global");
        f_scenes.setPointSize(16);//TODO: PORCENTAJE.
        f_scenes.setCapitalization(QFont::AllUppercase);
        f_scenes.setBold(true);

        QBrush br_base(QColor(10, 10, 10, 255));
        QBrush br_white(QColor(255, 255, 255, 255));
        QBrush br_black(QColor(0, 0, 0, 255));
        QBrush br_dark(QColor(40, 40, 40, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Base, br_base);
        p.setBrush(QPalette::Active, QPalette::Window, br_base);
        p.setBrush(QPalette::Active, QPalette::WindowText, br_white);
        p.setBrush(QPalette::Active, QPalette::Text, br_white);
        p.setBrush(QPalette::Active, QPalette::Button, br_base);
        p.setBrush(QPalette::Active, QPalette::ButtonText, br_white);
        p.setBrush(QPalette::Disabled, QPalette::Base, br_base);
        p.setBrush(QPalette::Disabled, QPalette::Window, br_base);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Text, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Button, br_base);
        p.setBrush(QPalette::Disabled, QPalette::ButtonText, br_black);

        QPalette p_scenes;
        p_scenes.setBrush(QPalette::Active, QPalette::Base, br_base);
        p_scenes.setBrush(QPalette::Active, QPalette::Window, br_base);
        p_scenes.setBrush(QPalette::Active, QPalette::WindowText, br_dark);
        p_scenes.setBrush(QPalette::Active, QPalette::Highlight, br_black);
        p_scenes.setBrush(QPalette::Active, QPalette::Button, br_base);
        p_scenes.setBrush(QPalette::Active, QPalette::ButtonText, br_white);
        p_scenes.setBrush(QPalette::Disabled, QPalette::Base, br_base);
        p_scenes.setBrush(QPalette::Disabled, QPalette::Window, br_base);
        p_scenes.setBrush(QPalette::Disabled, QPalette::WindowText, br_black);
        p_scenes.setBrush(QPalette::Disabled, QPalette::Highlight, br_base);
        p_scenes.setBrush(QPalette::Disabled, QPalette::Button, br_base);
        p_scenes.setBrush(QPalette::Disabled, QPalette::ButtonText, br_black);

        g_info = new QGroupBox(View);
        g_info->setStyleSheet("QGroupBox{ background-color: rgba(10, 10, 10, 255);"
                                  "border: 0; margin: 0; }"
                                  "QGroupBox::title { border: 0;margin: 0; }");
        g_info->setFlat(true);

        l_metadata = new QVBoxLayout;
        l_metadata->setSpacing(10);//TODO: PORCENTAJE.

//      QStringList metadata_titles;
//      metadata_titles << "NAME:" << "CASTING:" << "CATEGORIES:" << "QUALITY:" << "TIME:";
        for (int i = 0; i < 5; i++) {
            lb_metadata[i] = new QLabel(g_info);
            lb_metadata[i]->setFont(f);
            lb_metadata[i]->setPalette(p);
            //lb_metadata[i]->setText("<font color='#282828'> "+metadata_titles[i]+" </font>"+(**_data)[((index * 17) + i)]);
            lb_metadata[i]->setText((**_data)[((index * 17) + i)]);
            l_metadata->addWidget(lb_metadata[i]);
        }

        QPixmap px_cover(path);
        cover = new QLabel(g_info);
        cover->setPixmap(px_cover.scaled(335, 480, Qt::KeepAspectRatio));//

        QPixmap px_backCover(":/img-back_cover");
        backCover = new QLabel(g_info);
        backCover->setPixmap(px_backCover);//

        QString preview = "http://"+((**_data)[((index * 17) + 11)])+((**_data)[((index * 17) + 12)]);
        player = new Player(&preview, g_info);
        player->setFixedSize(QSize(700, 540));//TODO: PORCENTAJE.

        l_scenes = new QGridLayout;
        l_scenes->setMargin(0);

        QString scenes = (**_data)[((index * 17) + 16)];
        for (int i = 0; i < scenes.toInt(); i++) {
            lb_vector_scenes.push_back(new QLabel("WATCH SCENE ", g_info));
            lb_vector_scenes.last()->setFont(f_scenes);
            lb_vector_scenes.last()->setPalette(p_scenes);
            b_vector_scenes.push_back(new QPushButton(QIcon(":/icon-watch"), NULL, g_info));
            b_vector_scenes.last()->setToolTip("VIEW SCENE");
            b_vector_scenes.last()->setFlat(true);
            l_scenes->addWidget(lb_vector_scenes.last(), i, 0);
            l_scenes->addWidget(b_vector_scenes.last(), i, 1);
        }

        l_multimedia = new QHBoxLayout;
        l_multimedia->setSpacing(0);
        l_multimedia->setAlignment(Qt::AlignLeft);
        l_multimedia->addWidget(cover);
        l_multimedia->insertSpacing(1, 20);//TODO: PORCENTAJE.
        l_multimedia->addWidget(backCover);
        l_multimedia->insertSpacing(3, 40);//TODO: PORCENTAJE.
        l_multimedia->addWidget(player);
        l_multimedia->insertSpacing(5, 40);//TODO: PORCENTAJE.
        l_multimedia->addLayout(l_scenes);

        l_info = new QVBoxLayout(g_info);
        l_info->setContentsMargins(80, 12, 80, 12);//TODO: PORCENTAJE.
        l_info->setSpacing(250);//TODO: PORCENTAJE.
        l_info->setAlignment(Qt::AlignVCenter);
        l_info->addSpacing(10);//TODO: PORCENTAJE.
        l_info->addLayout(l_metadata);
        l_info->addLayout(l_multimedia);

        g_info->setLayout(l_info);

        sa_covers->setDisabled(true);
        sa_covers->hide();

        l->addWidget(g_info);
    }
};

namespace Ui {
    class PIG: public Ui_PIG {};
    class Authorization: public Ui_Authotization {};
    class Update: public Ui_Update {};
    class TopBar: public Ui_Topbar {};
    class Finder: public Ui_Finder {};
    class Setup: public Ui_Setup {};
    class View: public Ui_View {};
}

QT_END_NAMESPACE

#endif
