#include "../Core/Base.h"
#include "Sampler.h"

namespace gameplay
{

Sampler::Sampler() :
    _filterMag(Filter::eNearest),
    _filterMin(Filter::eNearest),
    _filterMip(Filter::eNearest),
    _addressModeU(AddressMode::eWrap),
    _addressModeV(AddressMode::eWrap),
    _addressModeW(AddressMode::eWrap),
    _borderColor(BorderColor::eBlackOpaque),
    _compareEnabled(true),
    _compareFunc(CompareFunc::eNever),
    _anisotropyEnabled(false),
    _anisotropyMax(0.0f),
    _lodMin(0.0f),
    _lodMax(0.0f),
    _lodMipBias(0.0f)
{
}

Sampler::~Sampler()
{
}

Sampler::Filter Sampler::getFilterMin() const
{
    return _filterMin;
}

Sampler::Filter Sampler::getFilterMag() const
{
    return _filterMag;
}

Sampler::Filter Sampler::getFilterMip() const
{
    return _filterMip;
}

Sampler::AddressMode Sampler::getAddressModeU() const
{
    return _addressModeU;
}

Sampler::AddressMode Sampler::getAddressModeV() const
{
    return _addressModeV;
}

Sampler::AddressMode Sampler::getAddressModeW() const
{
    return _addressModeW;
}

Sampler::BorderColor Sampler::getBorderColor() const
{
    return _borderColor;
}

bool Sampler::isCompareEnabled() const
{
    return _compareEnabled;
}

Sampler::CompareFunc Sampler::getCompareFunc() const
{
    return _compareFunc;
}

bool Sampler::isAnisotropyEnabled() const
{
    return _anisotropyEnabled;
}

float Sampler::getAnisotropyMax() const
{
    return _anisotropyMax;
}

float Sampler::getLodMin() const
{
    return _lodMin;
}

float Sampler::getLodMax() const
{
    return _lodMax;
}

float Sampler::getLodMipBias() const
{
    return _lodMipBias;
}

}
