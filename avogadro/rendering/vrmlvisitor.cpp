/******************************************************************************

  This source file is part of the Avogadro project.

  Copyright 2017 Kitware, Inc.

  This source code is released under the New BSD License, (the "License").

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

******************************************************************************/

#include "vrmlvisitor.h"

#include "ambientocclusionspheregeometry.h"
#include "cylindergeometry.h"
#include "linestripgeometry.h"
#include "meshgeometry.h"
#include "spheregeometry.h"

#include <iostream>
#include <ostream>

namespace Avogadro {
namespace Rendering {

using std::cout;
using std::string;
using std::endl;
using std::ostringstream;
using std::ostream;
using std::ofstream;

namespace {
ostream& operator<<(ostream& os, const Vector3f& v)
{
  os << v[0] << ", " << v[1] << ", " << v[2];
  return os;
}

ostream& operator<<(ostream& os, const Vector3ub& color)
{
  os << color[0] / 255.0f << ", " << color[1] / 255.0f << ", "
     << color[2] / 255.0f;
  return os;
}
}

VRMLVisitor::VRMLVisitor(const Camera& c)
  : m_camera(c), m_backgroundColor(255, 255, 255),
    m_ambientColor(100, 100, 100), m_aspectRatio(800.0f / 600.0f)
{
}

VRMLVisitor::~VRMLVisitor()
{
}

void VRMLVisitor::begin()
{
	/*
  // Initialise the VRML scene
  Vector3f cameraT = -(m_camera.modelView().linear().adjoint() *
                       m_camera.modelView().translation());
  Vector3f cameraX =
    m_camera.modelView().linear().row(0).transpose().normalized();
  Vector3f cameraY =
    m_camera.modelView().linear().row(1).transpose().normalized();
  Vector3f cameraZ =
    -m_camera.modelView().linear().row(2).transpose().normalized();

  double huge = 100;

  Vector3f light0pos =
    huge * (m_camera.modelView().linear().adjoint() * Vector3f(0, 1, 0));

  // Output the POV-Ray initialisation code
  ostringstream str;
  str << "global_settings {\n"
      << "\tambient_light rgb <" << m_ambientColor << ">\n"
      << "\tmax_trace_level 15\n}\n\n"
      << "background { color rgb <" << m_backgroundColor << "> }\n\n"
      << "camera {\n"
      << "\tperspective\n"
      << "\tlocation <" << cameraT.x() << ", " << cameraT.y() << ", "
      << cameraT.z() << ">\n"
      << "\tangle 70\n"
      << "\tup <" << cameraY.x() << ", " << cameraY.y() << ", " << cameraY.z()
      << ">\n"
      << "\tright <" << cameraX.x() << ", " << cameraX.y() << ", "
      << cameraX.z() << "> * " << m_aspectRatio << '\n'
      << "\tdirection <" << cameraZ.x() << ", " << cameraZ.y() << ", "
      << cameraZ.z() << "> }\n\n"

      << "light_source {\n"
      << "\t<" << light0pos[0] << ", " << light0pos[1] << ", " << light0pos[2]
      << ">\n"
      << "\tcolor rgb <1.0, 1.0, 1.0>\n"
      << "\tfade_distance " << 2 * huge << '\n'
      << "\tfade_power 0\n"
      << "\tparallel\n"
      << "\tpoint_at <" << -light0pos[0] << ", " << -light0pos[1] << ", "
      << -light0pos[2] << ">\n"
      << "}\n\n"

      << "#default {\n\tfinish {ambient .8 diffuse 1 specular 1 roughness .005 "
         "metallic 0.5}\n}\n\n";

  m_sceneData = str.str();
	*/
}

string VRMLVisitor::end()
{
  return m_sceneData;
}

void VRMLVisitor::visit(Drawable& geometry)
{
  // geometry.render(m_camera);
}

void VRMLVisitor::visit(SphereGeometry& geometry)
{
  ostringstream str;
  for (size_t i = 0; i < geometry.spheres().size(); ++i) {
    Rendering::SphereColor s = geometry.spheres()[i];

		str << "Transform {\n"
			<< "\ttranslation\t" << s.center[0] << "\t" << s.center[1] << "\t" << s.center[2]
			<< "\n\tchildren Shape {\n"
			<< "\t\tgeometry Sphere {\n\t\t\tradius\t" << s.radius << "\n\t\t}\n"
			<< "\t\tappearance Appearance {\n"
			<< "\t\t\tmaterial Material {\n"
			<< "\t\t\t\tdiffuseColor\t" << s.color[0] << "\t" << s.color[1]
			<< "\t" << s.color[2] << "\n\t\t\t}\n\t\t}\n\t}\n}\n";

  }
  m_sceneData += str.str();


}

void VRMLVisitor::visit(AmbientOcclusionSphereGeometry& geometry)
{
  // geometry.render(m_camera);
}

void VRMLVisitor::visit(CylinderGeometry& geometry)
{
  ostringstream str;
  for (size_t i = 0; i < geometry.cylinders().size(); ++i) {
    Rendering::CylinderColor c = geometry.cylinders()[i];

		double scale = 1.0;
		double x1, x2, y1, y2, z1, z2;
		x1 = c.end1[0];
		x2 = c.end2[0];
		y1 = c.end1[1];
		y2 = c.end2[1];
		z1 = c.end1[2];
		z2 = c.end2[2];

		double dx = x2 - x1;
		double dy = y2 - y1;
		double dz = z2 - z1;

		double length = sqrt(dx*dx + dy*dy + dz*dz);
		double tx = dx / 2 + x1;
		double ty = dy / 2 + y1;
		double tz = dz / 2 + z1;

		dx = dx / length;
		dy = dy / length;
		dz = dz / length;

		double ax, ay, az, angle;

		if (dy > 0.999) {
			ax = 1.0;
			ay = 0.0;
			az = 0.0;
			angle = 0.0;
		}
		else if (dy < -0.999) {
			ax = 1.0;
			ay = 0.0;
			az = 0.0;
			angle = 3.14159265359;
		}
		else {
			ax = dz;
			ay = 0.0;
			az = dx * -1.0;
			angle = acos(dy);
		}
		length = length / 2.0;

//		if ((radius*scale) < this->thinnestCyl) //keep track of the thinnest cylinder
//			this->thinnestCyl = radius*this->scale*2; //diameter

		str << "Transform {\n"
			<< "\ttranslation\t" << tx * scale << "\t" << ty * scale << "\t" << tz * scale
			<< "\n\tscale " << " 1 " << scale << " 1"
			<< "\n\trotation " << ax << " " << ay << " " << az << " " << angle
			<< "\n\tchildren Shape {\n"
			<< "\t\tgeometry Cylinder {\n\t\t\tradius\t" << c.radius * scale << "\n\t\t}\n"
			<< "\t\tappearance Appearance {\n"
			<< "\t\t\tmaterial Material {\n"
			<< "\t\t\t\tdiffuseColor\t" << c.color[0] << "\t" << c.color[1]
			<< "\t" << c.color[2] << "\n\t\t\t}\n\t\t}\n\t}\n}\n";

  }
  m_sceneData += str.str();
}

void VRMLVisitor::visit(MeshGeometry& geometry)
{

}

void VRMLVisitor::visit(LineStripGeometry& geometry)
{
  // geometry.render(m_camera);
}

} // End namespace Rendering
} // End namespace Avogadro
