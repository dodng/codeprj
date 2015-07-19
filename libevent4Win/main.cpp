#include <event2/event.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <WinSock2.h>

#pragma comment(lib,"wsock32.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"libevent.lib")
#pragma comment(lib,"libevent_core.lib")
#pragma comment(lib,"libevent_extras.lib")

#define MAX_LINE 16384
#define PORT 9999

void do_read(evutil_socket_t fd, short events, void *arg);
void do_write(evutil_socket_t fd, short events, void *arg);

struct fd_state {
	char buffer[MAX_LINE];
	size_t buffer_used;

	size_t n_written;
	size_t write_upto;

	struct event *read_event;
	struct event *write_event;
};

struct fd_state *alloc_fd_state(struct event_base *base, evutil_socket_t fd)
{
	struct fd_state *state =
		(struct fd_state *)malloc(sizeof(struct fd_state));
	if (!state) {
		return NULL;
	}

	state->read_event =
		event_new(base, fd, EV_READ | EV_PERSIST, do_read, state);
	if (!state->read_event) {
		free(state);
		return NULL;
	}

	state->write_event =
		event_new(base, fd, EV_WRITE | EV_PERSIST, do_write, state);
	if (!state->write_event) {
		event_free(state->read_event);
		free(state);
		return NULL;
	}

	assert(state->write_event);

	return state;
}

void free_fd_state(struct fd_state *state)
{
	event_free(state->read_event);
	event_free(state->write_event);
	free(state);
}

void do_read(evutil_socket_t fd, short events, void *arg)
{
	struct fd_state *state = (struct fd_state *)arg;
	char buf[1024];
	size_t result;
	while (1) {
		// assert(state->write_event);
		result = recv(fd, buf, sizeof(buf), 0);
		if (result <= 0)
			break;
		printf("[%s][%d]buf=[%s]len=[%d]\n", __FILE__, __LINE__, buf,
			result);
	}

	memcpy(state->buffer, "reply", sizeof("reply"));
	assert(state->write_event);
	event_add(state->write_event, NULL);
	state->write_upto = state->buffer_used;

	if (result == 0) {
		free_fd_state(state);
	} else if (result < 0) {
		if (errno == EAGAIN)    // XXXX use evutil macro
			return;
		perror("recv");
		free_fd_state(state);
	}
}

void do_write(evutil_socket_t fd, short events, void *arg)
{
	struct fd_state *state = (struct fd_state *)arg;

	printf("[%d]\n", __LINE__);
	//while (state->n_written < state->write_upto)
	{
		//ssize_t result = send(fd, state->buffer + state->n_written,
		//state->write_upto - state->n_written, 0);
		size_t result =
			send(fd, state->buffer, strlen(state->buffer), 0);
		if (result < 0) {
			if (errno == EAGAIN)    // XXX use evutil macro
				return;
			free_fd_state(state);
			return;
		}
		assert(result != 0);
		state->n_written += result;
	}

	//if (state->n_written == state->buffer_used)
	{
		state->n_written = state->write_upto = state->buffer_used = 1;
	}

	event_del(state->write_event);
}

void do_accept(evutil_socket_t listener, short event, void *arg)
{
	struct event_base *base = (struct event_base *)arg;
	struct sockaddr_storage ss;
	int slen = sizeof(ss);

	int fd = accept(listener, (struct sockaddr *)&ss, &slen);
	if (fd < 0) {        // XXXX eagain??
		perror("accept");
	} else if (fd > FD_SETSIZE) {
		closesocket(fd);    // XXX replace all closes with EVUTIL_CLOSESOCKET */
	} else {
		struct fd_state *state;
		evutil_make_socket_nonblocking(fd);
		state = alloc_fd_state(base, fd);

		assert(state);    /*XXX err */
		assert(state->write_event);
		event_add(state->read_event, NULL);
	}
}

void run(void)
{
	evutil_socket_t listener;
	struct sockaddr_in sin;
	struct event_base *base;
	struct event *listener_event;

	base = event_base_new();
	if (!base)
		return;        /*XXXerr */

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = 0;
	sin.sin_port = htons(PORT);

	listener = socket(AF_INET, SOCK_STREAM, 0);
	evutil_make_socket_nonblocking(listener);

#ifndef WIN32
	{
		int one = 1;
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &one,
			sizeof(one));
	}
#endif

	if (bind(listener, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
		perror("bind");
		return;
	}

	if (listen(listener, 16) < 0) {
		perror("listen");
		return;
	}

	listener_event =
		event_new(base, listener, EV_READ | EV_PERSIST, do_accept,
		(void *)base);
	/*XXX check it */
	event_add(listener_event, NULL);
	event_base_dispatch(base);
	event_base_free(base);

	return;
}

int main(int argc, char **argv)
{
	setvbuf(stdout, NULL, _IONBF, 0);
	run();

	return 0;
}
