#ifndef _KALZIUM_SETTINGSDIALOG_H_
#define _KALZIUM_SETTINGSDIALOG_H_

#include <kdialogbase.h>
#include <qtabwidget.h>

class KColorButton;
class QComboBox;
class QGridLayout;
class QLabel;
class QString;

class ColorsTabWidget : public QTabWidget
{
    Q_OBJECT
    public:
        ColorsTabWidget(QWidget *parent = 0, const char *name = 0/*, Kalzium *kalzium_tmp=0*/);
        QString indexnumber;
        
    private:
        QWidget *Stateofmatters, *Blocks, *Groups,*Acid;

        QLabel 
            *liquid, *solid, *vapor, *radioactive, *artificial,
        *s,*p,*d,*f, 
        *Group1, *Group2, *Group3, *Group4, *Group5, *Group6, *Group7, *Group8, 
        *acidic, *basic, *amphoteric, *neitherofthem;

        KColorButton 
            *liquid_b, *solid_b, *vapor_b, *radioactive_b , *artificial_b,
        *s_b,*p_b,*d_b,*f_b, 
        *Group1_b, *Group2_b, *Group3_b, *Group4_b, *Group5_b, *Group6_b, *Group7_b, *Group8_b, 
        *acidic_b, *basic_b, *amphoteric_b, *neitherofthem_b; 

        QComboBox *dropdown;
        QGridLayout *stateofmatters_layout, *blocks_layout, *groups_layout, *acid_layout;

    private slots:
        void saveColors();
        void setDefaultColors();
        void applyColors();
        void okColors();
};


class SettingsDialog : public KDialogBase
{
    Q_OBJECT
    public:
        SettingsDialog(QWidget *parent=0, const char *name=0);
    private:
        QFrame *colorTab;
        ColorsTabWidget *colorsTabWidget;
};

#endif
