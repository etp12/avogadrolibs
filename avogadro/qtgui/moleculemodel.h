/******************************************************************************

  This source file is part of the Avogadro project.

  Copyright 2013-2014 Kitware, Inc.

  This source code is released under the New BSD License, (the "License").

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

******************************************************************************/

#ifndef AVOGADRO_QTGUI_MOLECULEMODEL_H
#define AVOGADRO_QTGUI_MOLECULEMODEL_H

#include "avogadroqtguiexport.h"

#include <QtCore/QAbstractItemModel>

namespace Avogadro {
namespace QtGui {

class Molecule;

/**
 * @class MoleculeModel moleculemodel.h <avogadro/qtgui/moleculemodel.h>
 * @brief A model containing molecules.
 * @author Marcus D. Hanwell
 */

class AVOGADROQTGUI_EXPORT MoleculeModel : public QAbstractItemModel
{
  Q_OBJECT

public:
  explicit MoleculeModel(QObject *parent = 0);

  QModelIndex parent(const QModelIndex &child) const;
  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;

  Qt::ItemFlags flags(const QModelIndex &index) const;

  bool setData(const QModelIndex &index, const QVariant &value, int role);
  QVariant data(const QModelIndex &index, int role) const;

  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const;

  void clear();

  QList<Molecule *> molecules() const;
  QList<Molecule *> activeMolecules() const;

signals:
  void moleculeStateChanged(Avogadro::QtGui::Molecule *);

public slots:
  void addItem(Avogadro::QtGui::Molecule *item);
  void removeItem(Avogadro::QtGui::Molecule *item);
  void itemChanged();

private:
  QList<Molecule *> m_molecules;
};

} // End QtGui namespace
} // End Avogadro namespace

#endif // AVOGADRO_QTGUI_MOLECULEMODEL_H
