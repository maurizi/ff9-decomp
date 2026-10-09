# Near misses and puzzles (candidates for a dedicated hard-case pass)

Functions agents got close on but couldn't finish. Drafts for main are in the main checkout's
build/drafts/main/ (gitignored; v*.c). Append as agents report.

## main
- Register allocation / scheduling / branch order (>=90%): 80018F50 (126/128), 8002C308, 80029058,
  8004F2B4, 80054E88, 80057534, 8001C7FC, 80021B34, 800221C0, 800554D4, 80055230, 800343C0, 8005F718,
  800616F8, 8002FED4, 8002ECA0, 8002E718, 80062118, 800621D4, 8005B1B8, 8005AEC8, 8002B644.
- Order dependency: 800577EC / 80057898 match alone but break 800584C8; need 800579B4, 80057D80,
  80058118 matched first (work in address order).
- Loop shapes (search loop without gcc's empty-list shortcut; bit scans): 8004B040, 8004AE00,
  8004B098, 8004B4C4, 80038150, 80038278, 8004B85C, 8004AB7C, 80037738, 80049B48, 8002AE7C.
