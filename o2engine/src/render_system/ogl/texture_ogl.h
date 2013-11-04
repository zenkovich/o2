#ifndef TEXTURE_OGL_H
#define TEXTURE_OGL_H

#include "../texture_base_interface.h"
#include "ogl.h"

OPEN_O2_NAMESPACE

class grTexture:public grTextureBaseInterface
{
	friend class grRenderSystem;
	friend class grRenderTarget;

	GLuint mHandle; /**< Texture handle. */

public:
	/** ctor. */
	grTexture();

	/** ctor 
	 *  @size - size of texture
	 *  @format - texture format
	 *  @usage - texture usage. */
	grTexture(grRenderSystem* renderSystem, const vec2f& size, grTexFormat::type format = grTexFormat::DEFAULT, 
			  grTexUsage::type usage = grTexUsage::DEFAULT);

	/** ctor. Creates texture from file. */
	grTexture(grRenderSystem* renderSystem, const std::string& fileName);

	/** dtor. */
	~grTexture();

	/** Creates texture 
	 *  @size - size of texture
	 *  @format - texture format
	 *  @usage - texture usage. */
	void create(grRenderSystem* renderSystem, const vec2f& size, 
	            grTexFormat::type format = grTexFormat::DEFAULT, 
		  	    grTexUsage::type usage = grTexUsage::DEFAULT);

	/** Creates texture from image. */
	void createFromImage(grRenderSystem* renderSystem, cImage* image);
		       
	/** Creates texture from file. */
	void createFromFile(grRenderSystem* renderSystem, const std::string& fileName);
		       
	/** Creates texture as render target. */
	void createAsRenderTarget(grRenderSystem* renderSystem, const vec2f& size, 
		                      grTexFormat::type format = grTexFormat::DEFAULT);
};

CLOSE_O2_NAMESPACE

#endif //TEXTURE_OGL_H