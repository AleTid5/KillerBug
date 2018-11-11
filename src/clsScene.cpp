#include "clsScene.h"

int clsScene::init(const char* path, unsigned int posX, unsigned int posY)
{
    setItems(1);  // Cantidad de imagenes 1.
    setI(0);      // Indice del vector de imagenes.

    // Cargar la imagen.
    error.set(load(path));

    setName(path);
    setX(posX);
    setY(posY);
    setI(0);

    return error.get();
}

int clsScene::init(const char path[100][100], unsigned int countOfImages, unsigned int posX, unsigned int posY)
{
    setItems(countOfImages);
    for (unsigned int i = 0; i < countOfImages; i++) {
        setI(i);
        error.set(load(path[i]));
        setName(path[i]);
        setX(posX);
        setY(posY);
        setI(i);
    }

    return error.get();
}

int clsScene::initText()
{
    error.set(this->text.init());

    error.set(this->text.loadFont("FONTS/FreeMono.ttf", 25));

    return 0;
}

void clsScene::loadWallpaper(clsScreen* screen, clsEvent* event)
{
    const char pathes[100][100] = {
        "IMAGES/landscapes/wallpaper.jpg",
        "IMAGES/landscapes/landscape.jpg"};

    error.set(this->init(pathes, 2, 0, 0));
    this->setI(0);
    this->paste(screen->getPtr());
    screen->refresh();
    clsScene beginText[2];
    error.set(beginText[0].init("IMAGES/landscapes/killer-text.jpg"));
    clsTimer timer;
    beginText[0].setOpacity(0);
    beginText[0].setX(screen->getWidth() / 2 - beginText[0].getWidth() / 2);
    beginText[0].setY(screen->getHeight() / 2 - beginText[0].getHeight() / 2);
    beginText[1].init("IMAGES/landscapes/start-text.jpg", 0, beginText[0].getY() + beginText[0].getHeight() + 50); // 50px debajo
    beginText[1].setOpacity(0);
    beginText[1].setX(screen->getWidth() / 2 - beginText[1].getWidth() / 2);

    while (beginText[0].getOpacity() < 255) {
        event->wasEvent();
        if (event->getKey() == KEY_ESCAPE) throw 0;
        this->paste(screen->getPtr());
        beginText[0].setOpacity(beginText[0].getOpacity() + 5);
        beginText[0].paste(screen->getPtr());
        screen->refresh();
    }

    bool ascendant = true;

    while (event->getKey() != KEY_ENTER) {
        event->wasEvent();

        if (event->getKey() == KEY_ESCAPE) throw 0;

        beginText[1].setOpacity(beginText[1].getOpacity() + (ascendant ? 5 : - 5));

        if (beginText[1].getOpacity() == 255 || beginText[1].getOpacity() == 0)
            ascendant = ! ascendant;

        this->paste(screen->getPtr());
        beginText[0].paste(screen->getPtr());
        beginText[1].paste(screen->getPtr());
        screen->refresh();
    }

    this->setI(1);
    this->paste(screen->getPtr());
    screen->refresh();
}

void clsScene::move(clsScreen* screen)
{
    this->setI(1);
    if (-1 * this->getX() == this->getWidth() - 1370) this->setX(-4);

    this->setX(this->getX() - 4);
    this->paste(screen->getPtr());

    clsScene statusBar;
    statusBar.init("IMAGES/landscapes/status-bar.png", 0, 0);
    statusBar.paste(screen->getPtr());

    this->showTimer(screen);
}

void clsScene::startTimer()
{
    this->timer.start();
}

void clsScene::setTime(int time)
{
    this->timer.setTime(time);
}

int clsScene::getTime()
{
    return this->timer.getPlayiedTime();
}

int clsScene::getOptionSelected()
{
    return this->optionSelected;
}

void clsScene::showTimer(clsScreen* screen)
{
    this->timer.update();
    this->text.setFontColor(WHITE);
    char number[5];
    itoa(this->timer.getState() / 1000, number, 10);
    this->text.write(number, screen->getWidth() / 2 - this->text.getWidth() / 2, 15, screen->getPtr());
}

void clsScene::writeText(clsScreen* screen, unsigned int killedNumber, unsigned int posX, unsigned int posY)
{
    char killedBugs[5];
    itoa(killedNumber, killedBugs, 10);
    this->text.setFontColor(WHITE);
    this->text.write(killedBugs, posX, posY, screen->getPtr());
}

void clsScene::writeText(clsScreen* screen, const char* text, unsigned int posX, unsigned int posY)
{
    char textArray[5];
    strcpy(textArray, text);
    this->text.setFontColor(WHITE);
    this->text.write(textArray, posX, posY, screen->getPtr());
}

void clsScene::showMenu(clsScreen* screen)
{
    const char pathes[100][100] = {
        "IMAGES/menu/startPost.png",
        "IMAGES/menu/startPostPlay.png",
        "IMAGES/menu/startPostContinue.png",
        "IMAGES/menu/startPostScore.png",
        "IMAGES/menu/startPostExit.png",
        "IMAGES/menu/startHelp.png"
    };

    clsScene* menu = new clsScene;
    menu->error.set(menu->init(pathes, 6, 300, 10));
    menu->setI(0);
    menu->paste(screen->getPtr());
    screen->refresh();
    this->dispatchOption(menu, screen);
}

void clsScene::dispatchOption(clsScene* menu, clsScreen* screen)
{
    clsEvent* event = new clsEvent;

    while (event->getMouseButton() != MOUSE_LEFT_BUTTON || menu->getI() == 0) {
        Uint16 posX = event->getCursorX();
        Uint16 posY = event->getCursorY();

        menu->setI(0);

        if (posX >= 465 && posX <= 610 && posY >= 10 && posY <= 150)
            menu->setI(5);
        if (posX >= 360 && posX <= 750 && posY >= 160 && posY <= 285)
            menu->setI(1);
        else if (posX >= 300 && posX <= 630 && posY >= 285 && posY <= 350)
            menu->setI(2);
        else if (posX >= 450 && posX <= 675 && posY >= 350 && posY <= 410)
            menu->setI(3);
        else if (posX >= 415 && posX <= 675 && posY >= 450 && posY <= 535)
            menu->setI(4);

        this->paste(screen->getPtr());
        menu->paste(screen->getPtr());
        screen->refresh();
        event->wasEvent();
    }

    this->optionSelected = menu->getI();

    if (optionSelected == 4) throw 0;
}