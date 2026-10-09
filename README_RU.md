# NEON AI UPSCALER v0.2 — GitHub build

Android-приложение для тестирования классического масштабирования изображения через нативный C++ код. Это пока не AI-модель и не перехватчик графики других игр.

## Сборка APK через GitHub Actions
1. Создай репозиторий на https://github.com/new с именем `NEON-AI-UPSCALER`.
2. Загрузи содержимое этого архива в корень репозитория, включая скрытый путь `.github/workflows/android-apk.yml`.
3. Открой Actions и запусти `Build NEON AI UPSCALER APK` или сделай commit в `main`.
4. После успешного запуска скачай артефакт `NEON-AI-UPSCALER-debug-apk` и распакуй его — внутри будет `app-debug.apk`.

Сборка здесь ещё не запускалась; APK появится только после успешного выполнения GitHub Actions.
