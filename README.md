# ft_irc (simple team notes)

This README explains, in a simple way:
- how `handle_input` works,
- and how the server connects with clients.

---

## 1) Simple explanation of this method

```cpp
void managerchannel::handle_input(const std::string &input, client &c)
```

This function reads one message from one client (`c`) and decides what to do.

### A) When message starts with `PING`

```cpp
if (input.compare(0,4,"PING") == 0)
```

- It means client is checking if server is alive.
- Server replies to the **same client** with `PONG`.
- In your code, it also sends separator lines before/after `PONG`.

So: `PING` -> server answers `PONG` only to sender.

### B) When message starts with `test`

```cpp
if (input.compare(0,4,"test") == 0)
```

- Server loops over all connected clients in `_clients`.
- It skips the sender (`if (it->first != c.fd)`).
- It looks for `:` in input (example: `test:hello`).
- Text after `:` is extracted and sent to all other clients.

So: `test:hello` from one client -> all **other** clients receive `hello`.

---

## 2) How server and clients connect (simple)

Make the server run and listen on port `44444`.
1. Run server (example port `44444`):
	- `./ircserv 44444 0000`
2. Server starts listening:
	- You see: `Server listening on port 44444`
3. Open client terminal(s) and connect with netcat:
	- `nc 127.0.0.1 44444`
4. Each connection creates a new client FD on server:
	- Example: `New client connected. FD: 4`, `FD: 5`
5. Test messages:
	- Client sends `PING` -> gets `PONG`
	- Client sends `test:how are you` -> other client receives `how are you`

This is the base idea of IRC communication:
- server accepts many clients,
- reads input from one client,
- replies to one client or broadcasts to others.

---

## 3) Screenshot of your test

The following screenshot shows your server + two clients test:

![ft_irc server-client test](image/Screenshot%20from%202026-03-06%2020-15-15.png)

---

## 4) How to handle other commands (example: `JOIN`)

You can handle other IRC commands with the same idea you used for `PING` and `test`:

1. Check command name at start of input.
2. Parse needed argument(s) (for `JOIN`, parse channel name like `#general`).
3. Validate data (channel exists or create it).
4. Add client to channel members list.
5. Notify this client and other channel members.

### Simple example

Client sends:
- `JOIN #general`

Server should do:
- Find or create channel `#general`
- Add current client (`c`) to this channel
- Send confirmation to sender (example: `:server JOIN #general`)
- Broadcast to channel members that this user joined

In short: command handling is always **detect -> parse -> validate -> update state -> send response**.

