# Збірка на macOS

## Середовище

- Модель: MacBook Neo
- Чип: Apple Silicon (Apple A18 Pro, arm64)
- macOS: 26.7.1 (збірка 25G241)

## Встановлення інструментів

Command Line Tools (дають clang, make і git) ставляться командою:

    xcode-select --install

На цьому Mac вони вже були встановлені, тому нічого додатково ставити не довелось.

## Збірка і запуск

    git clone https://github.com/12nastya08a-beep/sound-messenger
    cd sound-messenger
    git checkout setup-skeleton
    make demo_tone
    ./demo_tone

Makefile сам визначає macOS і підставляє системні бібліотеки CoreFoundation, CoreAudio та AudioToolbox.

## Результат

Тон 1000 Гц пролунав приблизно секунду. Окремого дозволу на звук macOS не запитувала.

## Запасна команда (якщо make не спрацює)

    clang -Ithird_party demo/demo_tone.c -o demo_tone -framework CoreFoundation -framework CoreAudio -framework AudioToolbox -lpthread -lm
