SDL_Texture* TextureManager::LoadTexture(const char* texture) {
    SDL_Surface* tempSurface = IMG_Load(texture);
    if (!tempSurface) {
        std::cerr << "Failed to load texture: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    Uint32 colorkey = SDL_MapRGB(tempSurface->format, 255, 0, 255);
    SDL_SetColorKey(tempSurface, SDL_TRUE, colorkey);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    return tex;
}
