#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "main.h"

#define BUFSIZE 1024

/**
 * error_close - prints an error message and exits when close fails
 * @fd: file descriptor to close
 */

void error_close(int fd)
{
	dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", fd);
	exit(100);
}

/**
 * error_read - prints an error message and exits when read fails
 * @file: name of the file that could not be read
 * @fd_from: file descriptor of the source file
 * @fd_to: file descriptor of the destination file
 */

void error_read(char *file, int fd_from, int fd_to)
{
	dprintf(STDERR_FILENO, "Error: Can't read from file %s\n", file);
	if (close(fd_from) == -1)
		error_close(fd_from);
	if (close(fd_to) == -1)
		error_close(fd_to);
	exit(98);
}

/**
 * error_write - prints an error message and exits when write fails
 * @file: name of the file that could not be written to
 * @fd_from: file descriptor of the source file
 * @fd_to: file descriptor of the destination file
 */

void error_write(char *file, int fd_from, int fd_to)
{
	dprintf(STDERR_FILENO, "Error: Can't write to %s\n", file);
	if (close(fd_from) == -1)
		error_close(fd_from);
	if (close(fd_to) == -1)
		error_close(fd_to);
	exit(99);
}

/**
 * main - copies the content of a file to another file
 * @argc: number of command-line arguments
 * @argv: array of command-line arguments
 * Return: 0 on success, or one of the error codes on failure
 */

int main(int argc, char *argv[])
{
	int fd_from, fd_to;
	ssize_t n_read, n_written;
	char buffer[BUFSIZE];
	char *file_from, *file_to;

	if (argc != 3)
	{
		dprintf(STDERR_FILENO, "Usage: cp file_from file_to\n");
		exit(97);
	}

	file_from = argv[1];
	file_to = argv[2];

	fd_from = open(file_from, O_RDONLY);
	if (fd_from == -1)
		error_read(file_from, fd_from, fd_to);

	fd_to = open(file_to, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (fd_to == -1)
		error_write(file_to, fd_from, fd_to);

	while ((n_read = read(fd_from, buffer, BUFSIZE)) > 0)
	{
		n_written = write(fd_to, buffer, n_read);
		if (n_written == -1)
			error_write(file_to, fd_from, fd_to);
	}

	if (n_read == -1)
		error_read(file_from, fd_from, fd_to);

	if (close(fd_from) == -1)
		error_close(fd_from);

	if (close(fd_to) == -1)
		error_close(fd_to);

	return (0);
}
