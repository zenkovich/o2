## Менеджмент памяти
Сейчас используется ручной подход к памяти, с возможностью анализа количества аллокаций по месту их выделения. 

Для этого переопределены функции new() с параметрами исходника и номера строки. Для удоства они спрятаны в макрос mnew (managed new), который регистрирует аллокацию.

Далее можно вывести список мест аллокации, осортированный по сумме аллокаций:
`o2::MemoryManager::DumpInfo`.

Таким образом можно вычислить утечки и расход памяти

## Ссылки и GC
Я планирую перейти на использование умных указателей, с сохранением ссылки на счетчик внутри обёекта. Это будет представлено в типе Ref<>, который уже реализован в некотором виде для текстур, ассетов и акторов.

К нему будет поставляться отладочный GC, который сможет строить дерево аллокаций и позволит более подробно изучать расход памяти. И устранит ошибки с утечками из-за ручного управления памятью