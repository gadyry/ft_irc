```
                ┌──────────────┐
                │    Server    │
                │──────────────│
                │ - serv_fd    │
                │ - port       │
                │ - password   │
                │              │
                │ owns         │
                │              │
                │ clients map  │──────┐
                │ channels map │───┐  │
                └──────────────┘   │  │
                                   │  │
                                   ▼  ▼
                          ┌──────────┐   ┌──────────┐
                          │  Client  │   │  Client  │
                          │──────────│   │──────────│
                          │ fd       │   │ fd       │
                          │ nick     │   │ nick     │
                          │ user     │   │ user     │
                          │ buffer   │   │ buffer   │
                          └──────────┘   └──────────┘
                                ▲             ▲
                                │ references  │
                                │ (NO OWNERS) │
                         ┌──────────────────────────┐
                         │         Channel          │
                         │──────────────────────────│
                         │ name (#general)          │
                         │ topic                    │
                         │ members: Client*         │
                         │ operators: Client*       │
                         └──────────────────────────┘

```