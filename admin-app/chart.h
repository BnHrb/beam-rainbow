#ifndef CHART_H
#define CHART_H

#include <string>
#include <QSqlQuery>


class Chart
{
public:
    QString population[3] = {"Tout le monde","Femme seulement","Homme seulement"};
    QString categ[6] = {"Sexe","Tranche d'âge","Taille de la famille", "Nombre d'enfants", "Nombre d'animaux","Revenus"};
    Chart();

    Chart * load_all_chart(int id_sondage);
    bool save(); //save chart in db
    /* setter */
    void set_id_sondage(int value);
    void set_id_question(int value);
    void set_reponse_given(int value);
    void set_reponse_spe(bool value);
    void set_moyenne(bool value);
    void set_pourcentage(bool value);
    void set_pop_cible(QString value);
    void set_axe_abscisse(QString value);

    /*getter*/
    int set_id_sondage();
    int set_id_question();
    int set_reponse_given();
    bool set_reponse_spe();
    bool set_moyenne();
    bool set_pourcentage();
    QString set_pop_cible();
    QString set_axe_abscisse();


private:
    int id_sondage, id_question, reponse_given;
    bool reponse_spe, moyenne, pourcentage;
    QString pop_cible, axe_abscisse;
};

#endif // CHART_H
