#include "parse/parser.h"
#include "minishell.h"

void    get_input(t_cmd *command)
{
	
    t_cmd *cmd;
	t_redi *in;
	t_redi *out;

    int     i;
    int     x;
	
    cmd = command;
	printf("----------TABLE-------------\n");
	i = 0;
	while (cmd)
    {
        x = 0;
        while (cmd->cmd && cmd->cmd[x])
        {
            printf("[node:%d] cmd[%d] = %s\n", i, x, cmd->cmd[x]);
            x++;
        }
		printf("[node:%d] pipe   = %d\n", i,cmd->pipe);
		printf("[node:%d] er     = %d\n", i,cmd->err);
		printf("[node:%d] type     = %d\n", i,cmd->type);
        if (cmd->in)
        {
			in = cmd->in;
			while (in)
			{
				printf("-------------in-------------\n");
				printf("type = %d\n", in->type);
				printf("file = %s\n", in->file);
				printf("m_expd = %d\n", in->must_exp);
				in = in->next;
			}
        }
        if (cmd->out)
        {
			out = cmd->out;
			while (out)
			{
			  printf("-------------out------------\n");
			  printf("type = %d\n", out->type);
			  printf("file = %s\n", out->file);
			  printf("m_expd = %d\n", out->must_exp);
			  out = out->next;
			}
        }
        printf("----------------------------\n");
		i++;
		cmd = cmd->next;
	}
	printf("\n");
}


void	check_cmd(t_cmd *cmd)
{
	char	**substr;
	char	*temp;
	int		i;
	if (cmd->type == WORD && ft_strchr_2(cmd->cmd[0], ' '))
	{
		
		substr = ft_split(cmd->cmd[0], ' ');
		ft_free(cmd->cmd);
		i = 0;
		while(substr[i])
		temp = cmd->cmd[0];
		cmd->cmd[0] = ft_strdup(substr[0]);
		free(temp);
		if (substr[1])
		{
			temp = cmd->cmd[1];
			cmd->cmd[1] = ft_strdup(substr[1]);
			free(temp);
		}
		ft_free_tab2(substr);
		printf(">>>> %s\n", substr[0]);
		printf(">>>> %s\n", cmd->cmd[0]);
		// printf(">>>> %s\n", substr[1]);
		printf(">>>> %s\n", cmd->cmd[1]);
	}
}

void	ft_minishell(t_env **env, t_token **token, t_cmd **cmd)
{
	handler_expand(token, *env, *token);
	remove_spaces(token, *token);
	if (check_syntax(*token))
	{
		parse_cmd(token, cmd);
		check_cmd(*cmd);//a annuler
	}
	else
		ft_lstclear_t(token);
}

static void	check_args(int ac, char **av, t_env **dup_env, char **env)
{
	(void)av;
	if (ac != 1)
	{
		printf("errror nbrs argmnts");
		exit(1);
	}
	// parse_env(env, dup_env);
	init_env(dup_env,env);
}

int	main(int ac, char **av, char **env)
{
	t_token	*token;
	t_cmd	*cmd;
	t_env	*dup_env;
	char	*line;
	dup_env = NULL;
	check_args(ac, av, &dup_env, env);
	while (1)
	{
		token = NULL;
		cmd = NULL;
		line = readline("minishell> ");
		if (!line)
			to_exit();
		if (is_all_spaces(line))
		{
			free(line);
			continue ;
		}
		add_history(line);
		if (token_line(line, &token))
		{
			ft_minishell(&dup_env, &token, &cmd);
			// get_input(cmd);// a annuler
			execution(&dup_env, cmd);
			clear_cmds(&cmd);
		}
		free(line);
	}
}
