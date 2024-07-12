#include "philo.h"

void	*death_watch(void *arg)
{
	t_philos *philo;
	sem_t	*lock;

	philo = arg;
	lock = philo->end_lock;
	sem_wait(lock);
	sem_post(lock);
	philo->end = 1;
	return (NULL);
}

void child_process(t_philos *philo, pid_t *pid)
{
	pthread_t	thread;
	char		c;

	pthread_create(&thread, NULL, &death_watch, (void *)philo);
	while (!philo->end)
	{
		read(0, &c, 1);
		if (c == 'x')
			sem_post(philo->end_lock);
	}
	pthread_join(thread, NULL);
	sem_close(philo->end_lock);
	sem_close(philo->print_lock);
	sem_close(philo->forks);
	free(pid);
	exit (0);
}

int main(int argc, char **argv)
{
	t_philos *philo;
	pid_t	*pid;
	int		i;

	philo = init_philos(5, argc, argv);
	pid = malloc(sizeof(pid_t) * 3);
	sem_post(philo->end_lock);
	for (i = 0; i < 3; i++)
	{
		pid[i] = fork ();
		if (pid[i] == 0)
			child_process(philo, pid);
	}
	while (--i >= 0)
		waitpid(pid[i], NULL, 0);
	sem_close(philo->end_lock);
	sem_close(philo->print_lock);
	sem_close(philo->forks);
	sem_unlink(SEM_FORK);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_END);
	free (pid);
}