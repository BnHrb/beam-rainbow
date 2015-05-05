#include "chart.h"

Chart::Chart()
{    
}

Chart * Chart::load_all_chart(int id_sondage)
{

    return NULL;
}

/* setter */
void Chart::set_id_sondage(int value)
{
    this->id_sondage = value;
}

void Chart::set_id_question(int value)
{
    this->id_question = value;
}

void Chart::set_reponse_given(int value)
{
    this->reponse_given = value;
}

void Chart::set_id_reponse(int value)
{
    this->id_reponse = value;
}

void Chart::set_reponse_spe(bool value)
{
    this->reponse_spe = value;
}

void Chart::set_moyenne(bool value)
{
    this->moyenne = value;
}

void Chart::set_pourcentage(bool value)
{
    this->pourcentage = value;
}

void Chart::set_pop_cible(QString value)
{
    this->pop_cible = value;
}

void Chart::set_axe_abscisse(QString value)
{
    this->axe_abscisse = value;
}

/*getter*/
int Chart::set_id_sondage()
{
    return this->id_sondage;
}

int Chart::set_id_question()
{
    return this->id_question;
}

int Chart::set_reponse_given()
{
    return this->reponse_given;
}

int Chart::set_id_reponse()
{
    return this->id_reponse;
}

bool Chart::set_reponse_spe()
{
    return this->reponse_spe;
}

bool Chart::set_moyenne()
{
    return this->moyenne;
}

bool Chart::set_pourcentage()
{
    return this->pourcentage;
}

QString Chart::set_pop_cible()
{
    return this->pop_cible;
}

QString Chart::set_axe_abscisse()
{
    return this->axe_abscisse;
}
