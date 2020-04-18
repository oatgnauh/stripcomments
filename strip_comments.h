/*!
 * Method     StripComment
 * @brief	  一个有限状态机，用来删除掉json文件里面的注释
 * @param     std::string & text
 * @return    void
 */
void StripComment(std::string& text)
{
	enum TextParse {
		kClose,
		kSingleLineComment,
		kMutilLineComment,
		kNormaltext,
		kInQuotes
	};

	char pre_char = '\0';	//前一个字符
	TextParse state = kClose;
	int comment_begin_index = 0;
	for (size_t i = 0; i < text.size(); i++)
	{
		char this_char = text[i];

		switch (state)
		{
		case kClose:
		{
			if (this_char != '\0')
			{
				state = kNormaltext;
				break;
			}
		}
		case kNormaltext:
		{
			if (this_char == '/' && pre_char == '/')	// 以//开头的注释，遇到回车视为结束
			{
				state = kSingleLineComment;
				comment_begin_index = i - 1;
			}
			else if (this_char == '*' && pre_char == '/')	// 以/* 开头的注释，遇到*/视为结束
			{
				state = kMutilLineComment;
				comment_begin_index = i - 1;
			}
			break;
		}
		case kSingleLineComment:
		{
			if (this_char == '\n')		//结束单行注释
			{
				state = kNormaltext;
				int erase_count = i - comment_begin_index + 1;
				text.erase(comment_begin_index, erase_count);
				//erase 之后下标变动了，要修改
				i = comment_begin_index-1;
				this_char = text[i];
				comment_begin_index = 0;	
			}
			break;
		}
		case kMutilLineComment:
		{
			if (this_char == '/' && pre_char == '*')	// 结束多行注释
			{
				state = kNormaltext;
				int erase_count = i - comment_begin_index + 1;
				text.erase(comment_begin_index, erase_count);
				i = comment_begin_index - 1;		//重新指向注释开始的前一个字符
				this_char = text[i];
				comment_begin_index = 0;
			}
			break;
		}
		
		}
		pre_char = this_char;
	}
}
