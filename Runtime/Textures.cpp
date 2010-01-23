
#include "stdafx.h"

#ifndef APPRUNTIME

void CRunAnimation::GetReferencedImages(ImageHandleList& il)
{
	AnimationFrames::iterator f = frames.begin();
	AnimationFrames::iterator end = frames.end();

	for ( ; f != end; ++f)
		il.push_back(f->imgHandle);

	Animations::iterator a = subAnims.begin();
	Animations::iterator anim_end = subAnims.end();

	for ( ; a != anim_end; ++a)
		a->GetReferencedImages(il);

	a = Angles.begin();
	anim_end = Angles.end();

	for ( ; a != anim_end; ++a)
		a->GetReferencedImages(il);
}

void CRunAnimation::Load(Renderer& renderer)
{
	ImageHandleList::iterator i = referenced_images.begin();
	ImageHandleList::iterator end = referenced_images.end();

	for ( ; i != end; ++i) {
		if (!(*i)->IsInVRAM(renderer))
			(*i)->TransferToVRAM(renderer);

		(*i)->th->IncrementReferences();
	}

	// Now the textures are loaded we can set the various animation frame's width, height, texture etc
	UpdateFrameInfo();
}

void CRunAnimation::UpdateFrameInfo()
{
	// Now th is available we can set the frame's info from it
	AnimationFrames::iterator i = frames.begin();
	AnimationFrames::iterator end = frames.end();

	for ( ; i != end; ++i) {
		i->th = i->imgHandle->th;
		i->width = i->th->image_widthf;
		i->height = i->th->image_heightf;
	}

	// Run through sub anims and angles and apply recursively
	Animations::iterator a = subAnims.begin();
	Animations::iterator anim_end = subAnims.end();

	for ( ; a != anim_end; ++a)
		a->UpdateFrameInfo();

	a = Angles.begin();
	anim_end = Angles.end();

	for ( ; a != anim_end; ++a)
		a->UpdateFrameInfo();
}

void CRunAnimation::Unload(Renderer& renderer)
{
	ImageHandleList::iterator i = referenced_images.begin();
	ImageHandleList::iterator end = referenced_images.end();

	for ( ; i != end; ++i) {
		
		// Releases when reference count is 0.  Global objects persist and keep references to their anims.
		(*i)->ReleaseVRAM(renderer);
	}
}

// Free layout resources
void CRunLayout::Unload()
{
	FreeLayoutTextures();
}

// Create the texture for an image handle.
void ImageHandleInfo::TransferToVRAM(Renderer& renderer)
{
	CRASSERT(th == unallocated_texture);

	// Does not check for existing valid th; must check IsInVRAM first
	th = renderer.CreateTextureFromFileInMemory(pngFile, pngFileSize);
}

// Release VRAM if no longer needed.
void ImageHandleInfo::ReleaseVRAM(Renderer& renderer)
{
	// Is in VRAM
	if (th != unallocated_texture) {
		if (!renderer.TextureExists(th))
			th = unallocated_texture;
		else if (renderer.ReleaseTexture(th))		// Release from VRAM
			th = unallocated_texture;			// Got deallocated
	}
}

bool ImageHandleInfo::IsInVRAM(Renderer& renderer)
{
	// Not yet an allocated texture
	if (th == unallocated_texture)
		return false;

	// Horrible hack: sometimes textures (eg. canvas) can be released without the imagehandleinfo knowing about it
	// (its th is not set to unallocated_texture).  So we have to query the renderer itself if the texture exists...
	if (renderer.TextureExists(th))
		return true;
	else {
		th = unallocated_texture;	// uuurggh.  turns out it wasn't in VRAM after all, so mark th as unallocated...
		return false;
	}
}

void CRunLayout::LoadLayoutTextures()
{
	// If textures preloaded, no need to do anything
	if (has_loaded_textures)
		return;

	ImageHandleIterator i = used_image_handles.begin();
	ImageHandleIterator end = used_image_handles.end();

	for ( ; i != end; ++i) {
	
		// Not in VRAM: create it (textures are created with 0 references)
		if (!(*i)->IsInVRAM(pRuntime->renderer))
			(*i)->TransferToVRAM(pRuntime->renderer);
		
		// This layout now references this texture
		(*i)->th->IncrementReferences();
	}

	has_loaded_textures = true;
}

void CRunLayout::FreeLayoutTextures(bool force_free)
{
	if (!has_loaded_textures)
		return;

	// On application exit the layout is permanently ended so skip this check
	if (!force_free) {

		// Layout is set to load textures on an application scope: don't free textures when switching between layouts
		if (texture_loading == tl_load_on_app_start || ((texture_loading == tl_use_app_setting) && (pRuntime->texture_loading == CRuntime::tl_load_on_app_start)))
			return;
	}

	ImageHandleIterator i = used_image_handles.begin();
	ImageHandleIterator end = used_image_handles.end();

	for ( ; i != end; ++i) {

		// Layout no longer references this texture.  Global objects persist since they wont yet have released their
		// textures.
		(*i)->ReleaseVRAM(pRuntime->renderer);
	}

	has_loaded_textures = false;
}

#else // APPRUNTIME


void CRunAnimation::GetReferencedImages(ImageHandleList& il)
{
}

void CRunAnimation::Load(Renderer& renderer)
{
}

void CRunAnimation::UpdateFrameInfo()
{
}

void CRunAnimation::Unload(Renderer& renderer)
{
}

// Free layout resources
void CRunLayout::Unload()
{
}

void CRunLayout::LoadLayoutTextures()
{
}

void CRunLayout::FreeLayoutTextures(bool force_free)
{
}

#endif