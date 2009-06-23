/***************************************************************************
    copyright            : Kashyap R Puranik
    email                : kashthealien@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "massCalculator.h"
#include <QTimer>
// #include <formula.h>
// #include <chemistry.h>

// using namespace gcu;

massCalculator::massCalculator(QWidget * parent)
        : QFrame(parent)
{
    ui.setupUi(this);
	
    /**************************************************************************/
    //                       Mass Calculator set up
    /**************************************************************************/

    // Setup of the UI done
    
    //setup timer
	m_timer = new QTimer( this );
	m_timer->setSingleShot( true );
	
    // Initialise values
    
    // Initialisation of values done
    // Connect signals with slots
    connect(ui.formula, SIGNAL(textChanged(double)),
            this, SLOT( tick()));
   	connect( m_timer, SIGNAL( timeout()), 
            this, SLOT( formulaChanged()));
    /**************************************************************************/
    // 					mass Calculator setup complete
    /**************************************************************************/
    
}

massCalculator:: ~massCalculator()
{

}

// Calculates the mass of the molecule
void massCalculator::calculateMass(void)
{
//    double mass;
    	try {
//		char *format;
//			m_formula->SetFormula (ui.formula->text().toLatin1().data());
		}
/*		QString format = App->formula.GetMarkup ();
		gtk_label_set_markup (App->markup, format);

		format = g_strconcat (_("Raw formula:"), " \t", App->formula.GetRawMarkup (), NULL);
		gtk_label_set_markup (App->raw, format);

		bool artificial;
		DimensionalValue weight = App->formula.GetMolecularWeight (artificial);
		char *weightstr = (artificial)?
			g_strdup_printf ("(%.0f g.mol<sup>-1</sup>)",weight.GetAsDouble ()):
			g_strdup (weight.GetAsString ());			
		gtk_label_set_markup (App->weight, weightstr);

		// Composition
		gtk_list_store_clear (App->pclist);
		map<int,int> &raw = App->formula.GetRawFormula ();
		map<int,int>::iterator ri, riend = raw.end ();
		double pcent;
		map<string, int> elts;
		int nC = 0, nH = 0;
		map<int, int>::iterator j, jend = raw.end();
		for (j = raw.begin (); j != jend; j++) {
			switch ((*j).first) {
			case 1:
				nH = (*j).second;
				break;
			case 6:
				nC = (*j).second;
				break;
			default:
				elts[Element::Symbol((*j).first)] = (*j).second;
				break;
			}
		}
		GtkTreeIter iter;
		Element *elt;
		if (nC > 0) {
			elt = Element::GetElement (6);
			pcent = nC * elt->GetWeight ()->GetAsDouble () / weight.GetAsDouble () * 100.;
			weightstr = g_strdup_printf ((artificial)? "(%.0f)": "%.2f", pcent);
			gtk_list_store_append (App->pclist, &iter);
			gtk_list_store_set (App->pclist, &iter,
					  0, "C",
					  1, weightstr,
					  -1);
			g_free (weightstr);
		}
		if (nH > 0) {
			elt = Element::GetElement (1);
			pcent = nH * elt->GetWeight ()->GetAsDouble () / weight.GetAsDouble () * 100.;
			weightstr = g_strdup_printf ((artificial)? "(%.0f)": "%.2f", pcent);
			gtk_list_store_append (App->pclist, &iter);
			gtk_list_store_set (App->pclist, &iter,
					  0, "H",
					  1, weightstr,
					  -1);
			g_free (weightstr);
		}
		map<string, int>::iterator k, kend = elts.end ();
		for (k = elts.begin (); k != kend; k++) {
			nC = (*k).second;
			elt = Element::GetElement ((*k).first.c_str ());
			pcent = nC * elt->GetWeight ()->GetAsDouble () / weight.GetAsDouble () * 100.;
			weightstr = g_strdup_printf ((artificial)? "(%.0f)": "%.2f", pcent);
			gtk_list_store_append (App->pclist, &iter);
			gtk_list_store_set (App->pclist, &iter,
					  0, (*k).first.c_str (),
					  1, weightstr,
					  -1);
			g_free (weightstr);
		}
		// Isotopic pattern
		IsotopicPattern pattern;
		App->formula.CalculateIsotopicPattern (pattern);
		double *values, *x, *y;
		int n, mass, nb, min, max, i;
		mass = pattern.GetMinMass ();
		if (mass == 0) {
			// invalid pattern, do not display anything
			gtk_widget_hide (App->pattern_page);
			return;
		} else {
			gtk_label_set_text (App->monomass, pattern.GetMonoMass ().GetAsString ());
			gtk_widget_show (App->pattern_page);
			nb = pattern.GetValues (&values);
			// correct mean mass (for high molecular weights)
			double t = 0., m = 0;
			for (i = 0; i < nb; i++) {
				pcent = values[i] / nb;
				t += pcent;
				m += i * pcent;
			}
			mass = (int) rint (weight.GetAsDouble () - m / t);
			// do not display values < 0.1
			min = 0;
			while (values[min] < 0.1)
				min++;
			max = nb - 1;
			while (values[max] < 0.1)
				max--;
			max = max - min + 1;
			x = g_new (double, max);
			y  = g_new (double, max);
			for (i = 0, n = min; i < max; i++, n++) {
				x[i] = mass + n;
				y[i] = values[n];
			}
			GOData *data = go_data_vector_val_new (x, max, g_free);
			gog_series_set_dim (App->series, 0, data, &error);
			data = go_data_vector_val_new (y, max, g_free);
			gog_series_set_dim (App->series, 1, data, &error);
			g_free (values);
			// set axis bounds
			if (max - min < 30) {
				n = (30 - max + min) / 2;
				max += n;
				min -= n;
				if (mass + min < 0) {
					max -= mass + min;
					min = - mass;
				}
			}
			nb = (mass + min) / 10 * 10;
			n = (mass + min + max + 10) / 10 * 10;
			GogObject *obj = gog_object_get_child_by_role (GOG_OBJECT (App->chart),
					gog_object_find_role_by_name (GOG_OBJECT (App->chart), "X-Axis"));
			data = go_data_scalar_val_new (nb);
			gog_dataset_set_dim (GOG_DATASET (obj), GOG_AXIS_ELEM_MIN, data, &error);
			data = go_data_scalar_val_new (n);
			gog_dataset_set_dim (GOG_DATASET (obj), GOG_AXIS_ELEM_MAX, data, &error);
		}
	}*/
	catch (parse_error &error) {
//		int start, length;
//		char const *mess = error.what (start, length);
//		gtk_editable_select_region (GTK_EDITABLE (entry), start, start + length);
//		GtkWidget *w = gtk_message_dialog_new (GTK_WINDOW (data),
//							GTK_DIALOG_DESTROY_WITH_PARENT,
//							GTK_MESSAGE_ERROR,
//							GTK_BUTTONS_OK,
//							mess);
//		g_signal_connect_swapped (G_OBJECT (w), "response", G_CALLBACK (gtk_widget_destroy), G_OBJECT (w));
//		gtk_widget_show (w);
	}
    return;
}

// sets a single shot timer which waits for a second and then calls respective function
void massCalculator::tick(void)
{
    m_timer->stop();
    m_timer->start( 500 );
}

// occurs when the formula is changed
void massCalculator::formulaChanged(void)
{
	calculateMass ();
}

void massCalculator::error(int mode)
{
	mode = mode;
}

void massCalculator::debug(void)
{

}
#include "massCalculator.moc"


