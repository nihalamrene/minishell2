#include "../parse/parser.h"
#include "../minishell.h"
void	check_exp(t_token *tok, t_env *env)
{
	(void)env;
	if (tok->type == HYPHEN)
	{
		hyphen_exp(tok, env);
		if (!tok->content)
			return ;
	}
	else if ((tok->type == DOUBLE || tok->type == WORD)
		&& ft_strlen(tok->content) > 1)
	{
		if (*(tok->content) == '$')
			tok->expand = 1;
		expand_var(env, &tok->content);
	}
}

void	set_type(t_token *tok)
{
	if (tok->type == WORD)
		tok->type = SINGLE_EXP;
	else
		tok->type = SINGLE;
}

// void	here_doc_exp(t_token *token)
// {
// 	while (token)
// 	{
// 		if (*token->content == '$' && ft_strlen(token->content) == 1)
// 		{
// 			if (token->next && (token->next->type == DOUBLE
// 					|| token->next->type == SINGLE))
// 				token->content = ft_strdup("");
// 		}
// 		else if (ft_strlen(token->content) == 2 && token->type == OPERATOR
// 			&& ft_strcmp(token->content, "<<") == 0)
// 		{
// 			if (token->next && token->next->type == SPACEE)
// 			{
// 				if (token->next->next && token->next->next->type != PIPE
// 					&& token->next->next->type != OPERATOR)
// 					set_type(token->next->next);
// 			}
// 			else if (token->next && token->next->type != PIPE
// 				&& token->next->type != OPERATOR)
// 				set_type(token->next);
// 		}
// 		token = token->next;
// 	}
// }

int	join_str(t_token **token, t_token *tmp)
{
	if ((*token)->type == PIPE || (*token)->type == OPERATOR
		|| (*token)->type == SPACEE)
		return (0);
	if (!tmp || tmp->type == PIPE || tmp->type == OPERATOR
		|| tmp->type == SPACEE)
		return (0);
	else
	{
		tmp->content = ft_strjoin(tmp->content, (*token)->content);
		tmp->next = (*token)->next;
		ft_lstdelone_t(*token);
		*token = tmp->next;
		return (1);
	}
}

void	handler_expand(t_token **token, t_env *env, t_token *tok)
{
	t_token	*tmp;

	(void)env;
	(void)token;
	tmp = NULL;
	trim_quotes(*token);
	//here_doc_exp(*token);
	while (tok)
	{
		check_exp(tok, env);
		// check_tokens(tok);
		if (join_str(&tok, tmp) == 0)
		{
			tmp = tok;
			tok = tok->next;
		}
	}
}

int	exp_here(int curr, int next)
{
	if (curr == '$' && next == '?')
		return (1);
	if (curr == '$' && !must_expand(next))
		return (1);
	return (0);
}

int	count(char *str)
{
	int	i;

	i = 0;
	if (ft_isdigit(str[i]) || str[i] == '?')
		return (1);
	while (str[i] && !must_expand(str[i]))
		i++;
	return (i);
}

void	ft_free_2(char *str1, char *str2, char *str3)
{
	if (str1)
		free(str1);
	free(str2);
	free(str3);
}

void	expand_var(t_env *env, char **content)
{
	t_help	help;

	help.str = NULL;
	help.str1 = NULL;
	help.join = NULL;
	help.last_str = NULL;
	help.i = 0;
	help.prev = *content;
	while (help.prev[help.i] && !exp_here(help.prev[help.i],
			help.prev[help.i + 1]))
		help.i++;
	if (!help.prev[help.i] || !help.prev[help.i + 1])
		return ;
	if (help.i)
		help.str = ft_substr(help.prev, 0, help.i);
	help.j = help.i + 1 + count(help.prev + help.i + 1);
	help.str1 = get_value_of_exp(env, ft_substr(help.prev,
				help.i + 1, help.j - help.i - 1));
	help.join = ft_strjoin(help.str, help.str1);
	if (ft_strlen(help.prev + help.j))
		help.last_str = ft_substr(help.prev,
				help.j, ft_strlen(help.prev + help.j));
	*content = ft_strjoin(help.join, help.last_str);
	ft_free_2(help.last_str, help.prev, help.str1);
	expand_var(env, content);
}