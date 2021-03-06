//
// Copyright 2016 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "Apache License")
// with the following modification; you may not use this file except in
// compliance with the Apache License and the following modification to it:
// Section 6. Trademarks. is deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the trade
//    names, trademarks, service marks, or product names of the Licensor
//    and its affiliates, except as required to comply with Section 4(c) of
//    the License and to reproduce the content of the NOTICE file.
//
// You may obtain a copy of the Apache License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Apache License with the above modification is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the Apache License for the specific
// language governing permissions and limitations under the Apache License.
//
#ifndef PXR_IMAGING_HD_ST_SURFACE_SHADER_H
#define PXR_IMAGING_HD_ST_SURFACE_SHADER_H

#include "pxr/pxr.h"
#include "pxr/imaging/hdSt/api.h"
#include "pxr/imaging/hd/version.h"
#include "pxr/imaging/hdSt/shaderCode.h"
#include "pxr/imaging/hd/bufferSource.h"

#include "pxr/imaging/garch/gl.h"

#include "pxr/usd/sdf/path.h"

#include "pxr/base/vt/value.h"
#include "pxr/base/tf/token.h"

#include <boost/shared_ptr.hpp>

#include <memory>
#include <vector>

PXR_NAMESPACE_OPEN_SCOPE


class HdSceneDelegate;

using HdBufferArrayRangeSharedPtr = std::shared_ptr<class HdBufferArrayRange>;

typedef boost::shared_ptr<class HdStSurfaceShader> HdStSurfaceShaderSharedPtr;

using HdStResourceRegistrySharedPtr = 
    std::shared_ptr<class HdStResourceRegistry>;

/// \class HdStSurfaceShader
///
/// A scene-based SurfaceShader object.
///
/// When surface shaders are expresed in the scene graph, the HdSceneDelegate
/// can use this object to express these surface shaders in Storm. In addition
/// to the shader itself, a binding from the Rprim to the SurfaceShader must be
/// expressed as well.
class HdStSurfaceShader : public HdStShaderCode {
public:
    HDST_API
    HdStSurfaceShader();
    HDST_API
    ~HdStSurfaceShader() override;


    // ---------------------------------------------------------------------- //
    /// \name HdShader Virtual Interface                                      //
    // ---------------------------------------------------------------------- //
    HDST_API
    virtual std::string GetSource(TfToken const &shaderStageKey) const override;
    HDST_API
    virtual HdSt_MaterialParamVector const& GetParams() const override;
    HDST_API
    void SetEnabledPrimvarFiltering(bool enabled);
    HDST_API
    virtual bool IsEnabledPrimvarFiltering() const override;
    HDST_API
    virtual TfTokenVector const& GetPrimvarNames() const override;
    HDST_API
    virtual HdBufferArrayRangeSharedPtr const& GetShaderData() const override;
    HDST_API
    virtual TextureDescriptorVector GetTextures() const override;
    HDST_API
    virtual void BindResources(int program,
                               HdSt_ResourceBinder const &binder,
                               HdRenderPassState const &state) override;
    HDST_API
    virtual void UnbindResources(int program,
                                 HdSt_ResourceBinder const &binder,
                                 
                                 HdRenderPassState const &state) override;
    HDST_API
    virtual void AddBindings(HdBindingRequestVector *customBindings) override;
    HDST_API
    virtual ID ComputeHash() const override;

    HDST_API
    virtual TfToken GetMaterialTag() const override;


    /// Setter method for prim
    HDST_API
    void SetFragmentSource(const std::string &source);
    HDST_API
    void SetGeometrySource(const std::string &source);
    HDST_API
    void SetParams(const HdSt_MaterialParamVector &params);
    HDST_API
    void SetTextureDescriptors(const TextureDescriptorVector &texDesc);
    HDST_API
    void SetBufferSources(
        HdBufferSourceSharedPtrVector &bufferSources, 
        HdStResourceRegistrySharedPtr const &resourceRegistry);

    HDST_API
    void SetMaterialTag(TfToken const &materialTag);

    /// If the prim is based on asset, reload that asset.
    HDST_API
    virtual void Reload();

    /// Returns if the two shaders can be aggregated into the same draw batch.
    HDST_API
    static bool CanAggregate(HdStShaderCodeSharedPtr const &shaderA,
                             HdStShaderCodeSharedPtr const &shaderB);

protected:
    HDST_API
    void _SetSource(TfToken const &shaderStageKey, std::string const &source);

    HDST_API
    ID _ComputeHash() const;

private:
    std::string _fragmentSource;
    std::string _geometrySource;

    // Shader Parameters
    HdSt_MaterialParamVector       _params;
    HdBufferSpecVector          _paramSpec;
    HdBufferArrayRangeSharedPtr _paramArray;
    TfTokenVector               _primvarNames;
    bool                        _isEnabledPrimvarFiltering;

    mutable size_t              _computedHash;
    mutable bool                _isValidComputedHash;

    TextureDescriptorVector _textureDescriptors;

    TfToken _materialTag;

    // No copying
    HdStSurfaceShader(const HdStSurfaceShader &)                     = delete;
    HdStSurfaceShader &operator =(const HdStSurfaceShader &)         = delete;
};


PXR_NAMESPACE_CLOSE_SCOPE

#endif //PXR_IMAGING_HD_ST_SURFACE_SHADER_H
