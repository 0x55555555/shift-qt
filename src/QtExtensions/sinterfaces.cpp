#include "shift/QtExtensions/sinterfaces.h"

#if X_QT_INTEROP

#include "shift/TypeInformation/spropertyinformation.h"
#include "shift/Properties/sbaseproperties.h"
#include "shift/Properties/sbasepointerproperties.h"
#include "shift/Changes/shandler.inl"
#include "shift/sentity.h"
#include "QtGui/QColor"
#include "QtCore/QString"

namespace Shift
{

const QString g_positionName("__position");
const QString g_inputsPositionName("__inputsPosition");
const QString g_outputsPositionName("__outputsPosition");

Eks::Vector3D SBasicPositionInterface::positionGeneric(const Property * p, const NameArg &name) const
  {
  const Entity *ent = p->uncheckedCastTo<Entity>();
  const Vector3DProperty *prop = ent->findChild<Vector3DProperty>(name);
  if(prop)
    {
    return prop->value();
    }
  xAssert(!ent->findChild(name));
  return Eks::Vector3D::Zero();
  }

void SBasicPositionInterface::setPositionGeneric(Property * p, const Eks::Vector3D &val, const NameArg &name) const
  {
  class Initialiser : public PropertyInstanceInformationInitialiser
    {
  public:
    void initialise(PropertyInstanceInformation *inst)
      {
      inst->setMode(PropertyInstanceInformation::Internal);
      }
    };

  Entity *ent = p->uncheckedCastTo<Entity>();
  Vector3DProperty *prop = ent->uncheckedCastTo<Entity>()->findChild<Vector3DProperty>(name);
  if(!prop)
    {
    Initialiser init;
    prop = ent->addAttribute<Vector3DProperty>(std::numeric_limits<xsize>::max(), name, &init);
    }
  xAssert(prop);

  prop->assign(val);
  }

Eks::Vector3D SBasicPositionInterface::position(const Property *p) const
  {
  return positionGeneric(p, g_positionName);
  }

void SBasicPositionInterface::setPosition(Property *p, const Eks::Vector3D &val) const
  {
  setPositionGeneric(p, val, g_positionName);
  }

Eks::Vector3D SBasicPositionInterface::inputsPosition(const Property *p) const
  {
  return positionGeneric(p, g_inputsPositionName);
  }

void SBasicPositionInterface::setInputsPosition(Property *p, const Eks::Vector3D &val) const
  {
  setPositionGeneric(p, val, g_inputsPositionName);
  }

Eks::Vector3D SBasicPositionInterface::outputsPosition(const Property *p) const
  {
  return positionGeneric(p, g_outputsPositionName);
  }

void SBasicPositionInterface::setOutputsPosition(Property *p, const Eks::Vector3D &val) const
  {
  setPositionGeneric(p, val, g_outputsPositionName);
  }

Eks::Colour SBasicColourInterface::colour(const Property *t) const
  {
  return colour(t->typeInformation());
  }

Eks::Colour SBasicColourInterface::colour(const PropertyInformation *t) const
  {
  xAssertFail();
  if(t->inheritsFromType<Pointer>())
    {
    //const PropertyInformation *pointedTo = Pointer::pointedToType(t);
    //return colour(pointedTo);
    }

  xuint32 h = qHash(t->typeName());

  float lightness = (float)(h & 0xFF) / 0xFF;
  float saturation = (float)((h >> 8) & 0xFF) / 0xFF;
  float hue = (float)((h >> 16) & 0xFFF) / 0xFFF; // skip last nibble

  int hueI = hue * 360.0f; // 0.0 -> 360.0
  int satI = ((saturation / 2.0f) + 0.5f) * 255.0f; // 0.5 -> 1.0
  int ligI = ((lightness / 2.0f) + 0.25f) * 255.0f; // 0.25 -> 0.75

  QColor col = QColor::fromHsl(hueI, satI, ligI);

  return Eks::Colour(col);
  }

}

#endif
