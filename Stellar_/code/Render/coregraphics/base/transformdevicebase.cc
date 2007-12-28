//------------------------------------------------------------------------------
//  transformdevicebase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/base/transformdevicebase.h"

namespace Base
{
ImplementClass(Base::TransformDeviceBase, 'TDVB', Core::RefCounted);
ImplementSingleton(Base::TransformDeviceBase);

using namespace Util;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
TransformDeviceBase::TransformDeviceBase() :
    isOpen(false),
    dirtyFlags(0)/*,
    transforms(NumTransformTypes)*/
{
    transforms.resize(NumTransformTypes);
}

//------------------------------------------------------------------------------
/**
*/
TransformDeviceBase::~TransformDeviceBase()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
TransformDeviceBase::Open()
{
    s_assert(!this->IsOpen());
    this->isOpen = true;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
TransformDeviceBase::Close()
{
    s_assert(this->IsOpen());
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
TransformDeviceBase::SetProjTransform(const matrix44& m)
{
    this->transforms[Proj] = m;
    this->SetDirtyFlag(InvProj);
    this->SetDirtyFlag(ViewProj);
    this->SetDirtyFlag(ModelViewProj);
}

//------------------------------------------------------------------------------
/**
*/
void
TransformDeviceBase::SetViewTransform(const matrix44& m)
{
    this->transforms[View] = m;
    this->SetDirtyFlag(InvView);
    this->SetDirtyFlag(ViewProj);
    this->SetDirtyFlag(ModelView);
    this->SetDirtyFlag(InvModelView);
    this->SetDirtyFlag(ModelViewProj);
}

//------------------------------------------------------------------------------
/**
*/
void
TransformDeviceBase::SetModelTransform(const matrix44& m)
{
    this->transforms[Model] = m;
    this->SetDirtyFlag(InvModel);
    this->SetDirtyFlag(ModelView);
    this->SetDirtyFlag(InvModelView);
    this->SetDirtyFlag(ModelViewProj);
}

//------------------------------------------------------------------------------
/**
*/
void
TransformDeviceBase::UpdateInvProjTransform()
{
    s_assert(this->IsDirty(InvProj));
    this->transforms[InvProj] = matrix44::inverse(this->transforms[Proj]);
    this->ClearDirtyFlag(InvProj);
}

//------------------------------------------------------------------------------
/**
*/
void
TransformDeviceBase::UpdateInvViewTransform()
{
    s_assert(this->IsDirty(InvView));
    this->transforms[InvView] = matrix44::inverse(this->transforms[View]);
    this->ClearDirtyFlag(InvView);
}

//------------------------------------------------------------------------------
/**
*/
void
TransformDeviceBase::UpdateViewProjTransform()
{
    s_assert(this->IsDirty(ViewProj));
    this->transforms[ViewProj] = matrix44::multiply(this->transforms[View], this->transforms[Proj]);
    this->ClearDirtyFlag(ViewProj);
}

//------------------------------------------------------------------------------
/**
*/
void
TransformDeviceBase::UpdateInvModelTransform()
{
    s_assert(this->IsDirty(InvModel));
    this->transforms[InvModel] = matrix44::inverse(this->transforms[Model]);
    this->ClearDirtyFlag(InvModel);
}

//------------------------------------------------------------------------------
/**
*/
void
TransformDeviceBase::UpdateModelViewTransform()
{
    s_assert(this->IsDirty(ModelView));
    this->transforms[ModelView] = matrix44::multiply(this->transforms[Model], this->transforms[View]);
    this->ClearDirtyFlag(ModelView);
}

//------------------------------------------------------------------------------
/**
*/
void
TransformDeviceBase::UpdateInvModelViewTransform()
{
    s_assert(this->IsDirty(InvModelView));

    // the InvModel and InvView matrices are usually already computed,
    // so we can do a cheap multiply instead of an expensive inversion
    if (this->IsDirty(InvModel))
    {
        this->UpdateInvModelTransform();
    }
    if (this->IsDirty(InvView))
    {
        this->UpdateInvViewTransform();
    }
    this->transforms[InvModelView] = matrix44::multiply(this->transforms[InvView], this->transforms[InvModel]);
    this->ClearDirtyFlag(InvModelView);
}

//------------------------------------------------------------------------------
/**
*/
void
TransformDeviceBase::UpdateModelViewProjTransform()
{
    s_assert(this->IsDirty(ModelViewProj));
    if (this->IsDirty(ModelView))
    {
        this->UpdateModelViewTransform();
    }
    this->transforms[ModelViewProj] = matrix44::multiply(this->transforms[ModelView], this->transforms[Proj]);
    this->ClearDirtyFlag(ModelViewProj);
}

} // namespace Base
