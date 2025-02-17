## Система ввода, o2::EventSystem и o2::Input
`o2::EventSystem` занимается обработкой нажатия клавишь и мышки (или тачей). Она перенаправляет сообщения в нужные сущности, разрешая внутри себя проблемы отсечения и порядка отрисовки.

`o2::Input` предоставляет информацию о текущем состоянии систем ввода:
- нажатие клавиш: только что нажата, нажата когда-либо, только что отпущена
- нажание курсоров (тачей): нажатие, отпускание
- положение курсора, дельта курсора за кадр

### Обработка клавиш, o2::KeyboardEventsListener
Этот класс используется как интерфейс обработчика нажатий. От него можно наследоваться и перегрузить нужные функции. Сообщения о нажатии/отпускании клавишь приходят всем наследникам интерфейса автоматически

### Обработка курсора и тачей, o2::CursorAreaEventsListener
Этот интерфейс обрабатывает не все нажатия и события, а только при попадании курсора в него с условием перекрытия другими обработчиками тачей.

Наследовавшись от интерфейса, нужно перегрузить функцию `IsUnderPoint`, которая проверяет попадание точки в область, без учета отсечения. Например, попадание по какой-то геометрической форме или в графику. А так же необходимо вызывать функцию `OnDraw` в тот момент, когда сущность появляется графически на экране

Система ввода сама занимается вопросом отсечения и перекрытия слушателей друг другом.

Так же нужно перегрузить нужные функции-сообщения курсоров и тачей:
-`OnCursorPressed` - курсор нажат на сущности
-`OnCursorReleased` - курсор отпущен, не обязательно над ней
-`OnCursorPressBreak` - прерывание нажатого курсора (системное, выход за область экрана и тп)
-`OnCursorPressedOutside` - курсор нажат снаружи
-`OnCursorReleasedOutside` - курсор отпущен снаружи
-`OnCursorStillDown` - курсор все еще нажат, не факт что внутри
-`OnCursorMoved` - курсор переместился внутри области
-`OnCursorEnter` - курсор вошел в область
-`OnCursorExit` - курсор вышел из области
-`OnCursorDblClicked` - двойной клик по области