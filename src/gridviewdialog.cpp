/*
Dwarf Therapist
Copyright (c) 2009 Trey Stout (chmod)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "gridviewdialog.h"
#include "ui_gridviewdialog.h"
#include "viewmanager.h"
#include "gridview.h"
#include "viewcolumnset.h"
#include "defines.h"

GridViewDialog::GridViewDialog(ViewManager *mgr, GridView *view, QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::GridViewDialog)
	, m_view(view)
	, m_manager(mgr)
{
	ui->setupUi(this);
	populate_combo_box();

	if (m_view) { // looks like this is an edit...
		ui->le_name->setText(view->name());
		draw_sets();
	}
	ui->list_sets->installEventFilter(this);

	connect(ui->le_name, SIGNAL(textChanged(const QString &)), this, SLOT(check_name(const QString &)));
}

QString GridViewDialog::name() {
	 return ui->le_name->text();
}

QStringList GridViewDialog::sets() {
	QStringList retval;
	for(int i = 0; i < ui->list_sets->count(); ++i) {
		retval << ui->list_sets->item(i)->text();
	}
	return retval;
}

void GridViewDialog::draw_sets() {
	ui->list_sets->clear();
	foreach(ViewColumnSet *set, m_view->sets()) {
		QListWidgetItem *item = new QListWidgetItem(set->name(), ui->list_sets);
		item->setBackgroundColor(set->bg_color());
	}
}

void GridViewDialog::populate_combo_box() {
	ui->cb_sets->clear();
	foreach(ViewColumnSet *set, m_manager->sets()) {
		if (m_view && !m_view->sets().contains(set)) {
			ui->cb_sets->addItem(set->name());
		}
	}
}

bool GridViewDialog::eventFilter(QObject *, QEvent *e) {
	if (e->type() == QEvent::ChildRemoved) {
		order_changed(); // we're just here to help the signals along since this shit is broken in Qt...
	}
	return false; // don't actually interrupt anything
}

void GridViewDialog::order_changed() {
	LOGD << "ORDER CHANGED!";
}

void GridViewDialog::check_name(const QString &name) {
	ui->buttonBox->setDisabled(name.isEmpty());
}