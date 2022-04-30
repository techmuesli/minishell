// !!!!!

#include "../../minishell.h"

static void	export_print_list(t_data *data);
static int	get_key_value_pair(char *argument, t_environment *entry);
static void	update_existing_entry(t_vector *env, int index, char *new_value);
static char	*remove_surrounding_quotes(char *input);
static int	is_valid_entry_name(char *name);

// without any arguments, this prints all variables available
int	builtin_export(t_data *data, char **args)
{
	int				i;
	int				update;
	int				index;
	t_environment	entry;

	if (args[1] == NULL)
	{
		export_print_list(data);
		return (1);
	}
	i = 1;
	while (args[i] != NULL)
	{
		update = get_key_value_pair(args[i], &entry);
		if (is_valid_entry_name(entry.name) == 0)
		{
			printf ("export: '%s': not a valid identifier\n", entry.name);
			free(entry.name);
			if (entry.value != NULL)
				free(entry.value);
			return (i);
		}
		if (ft_strcmp("PATH", entry.name) == 0)
		{
			free_path(data);
			data->path = ft_split(entry.value, ':');
		}
		index = get_entry_index(data->environment, entry.name);
		if (index >= 0)
		{
			if (update == 1)
				update_existing_entry(data->environment, index, entry.value);
			free(entry.name);
			free(entry.value);
		}
		else
		{
			entry.initial_index = get_new_initial_index(data->environment);
			vector_add(data->environment, &entry);
			add_to_envp(data, entry.name, entry.value);
		}
		i++;
	}
	sort_all_entries(data->environment);
	return (i);
}

static void	export_print_list(t_data *data)
{
	int				i;
	t_environment	*entry;

	i = 0;
	entry = vector_get(data->environment, i);
	while (entry != NULL)
	{
		if (ft_strlen(entry->value) == 0)
			printf ("declare -x %s\n", entry->name);
		else
			printf ("declare -x %s=\"%s\"\n", entry->name, entry->value);
		i++;
		entry = vector_get(data->environment, i);
	}
}

static int	get_key_value_pair(char *argument, t_environment *entry)
{
	int	length_name;
	int	length_value;
	int	update;

	update = 1;
	argument = remove_surrounding_quotes(argument);
	length_name = get_name_length(argument);
	entry->name = malloc(length_name + 1);
	if (entry->name == NULL)
		exit(1);
	ft_strlcpy(entry->name, argument, length_name + 1);
	length_value = 0;
	if (argument[length_name] == '=')
		length_value = ft_strlen(&argument[length_name + 1]);
	else
		update = 0;
	entry->value = ft_calloc(length_value + 1, sizeof(char));
	if (entry->value == NULL)
		exit(1);
	if (length_value != 0)
		ft_strlcpy(entry->value, &argument[length_name + 1], length_value + 1);
	free(argument);
	return (update);
}

// removes quotes around the name and the value of the current argument
static char	*remove_surrounding_quotes(char *input)
{
	char	*result;
	int		length;
	int		length_name;

	length = ft_strlen(input);
	length_name = get_name_length(input);
	result = malloc(length + 1);
	if (result == NULL)
		exit(1);
	if ((input[0] == '"' && input[length_name - 1] == '"')
		|| (input[0] == '\'' && input[length_name - 1] == '\''))
		ft_strlcpy(result, &input[1], length_name - 2 + 1);
	else
		ft_strlcpy(result, input, length_name + 1);
	if (length_name != length)
	{
		result[ft_strlen(result) + 1] = '\0'; // !!!!!
		result[ft_strlen(result)] = '=';
		if ((input[length_name + 1] == '"' && input[length - 1] == '"')
			|| (input[length_name + 1] == '\'' && input[length - 1] == '\''))
			ft_strlcpy(&result[ft_strlen(result)], &input[length_name + 2], length - length_name - 2);
		else
			ft_strlcpy(&result[ft_strlen(result)], &input[length_name + 1], length - length_name);
	}
	//free(input);
	return (result);
}

// returns the index of an existing entry name
int	get_entry_index(t_vector *env, char *name)
{
	int				i;
	t_environment	*entry;

	i = 0;
	entry = vector_get(env, i);
	while (entry != NULL)
	{
		if (ft_strcmp(name, entry->name) == 0)
			return (i);
		i++;
		entry = vector_get(env, i);
	}
	return (-1);
}

static void	update_existing_entry(t_vector *env, int index, char *new_value)
{
	t_environment	*entry;

	if (new_value != NULL)
	{
		entry = vector_get(env, index);
		if (entry == NULL)
			return ;
		free(entry->value);
		entry->value = malloc(ft_strlen(new_value) + 1);
		if (entry->value == NULL)
			exit(1);
		ft_strlcpy(entry->value, new_value, ft_strlen(new_value) + 1);
	}
}

// if all variables are removed at some point, 
// the starting index will be 1, not 0 anymore
int	get_new_initial_index(t_vector *env)
{
	int				i;
	int				highest_index;
	t_environment	*entry;

	highest_index = 0;
	i = 0;
	entry = vector_get(env, i);
	while (entry != NULL)
	{
		if (entry->initial_index > highest_index)
			highest_index = entry->initial_index;
		i++;
		entry = vector_get(env, i);
	}
	highest_index++;
	return (highest_index);
}

static int	is_valid_entry_name(char *name)
{
	if (ft_isalpha(*name) == 0 && *name != '_')
		return (0);
	while (*name != '\0')
	{
		if (ft_isalnum(*name) == 0 && *name != '_')
			return (0);
		name++;
	}
	return (1);
}
