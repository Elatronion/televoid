#include "text.h"

//GLuint VAO, VBO;
GLuint TEXT_VAO, TEXT_VBO;
Character Characters[128];

void televoidTextCreate(const char* font_path)
{
	hgeUseShader(hgeResourcesQueryShader("text"));
	// Compile and setup the shader

	// FreeType
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		printf("ERROR::FREETYPE: Could not init FreeType Library\n");

	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, font_path, 0, &face))
		printf("ERROR::FREETYPE: Failed to load font\n");

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			printf("ERROR::FREETYTPE: Failed to load Glyph\n");
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
			);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Characters[c].TextureID = texture;
		Characters[c].Size.x = face->glyph->bitmap.width;
		Characters[c].Size.y = face->glyph->bitmap.rows;
		Characters[c].Bearing.x = face->glyph->bitmap_left;
		Characters[c].Bearing.y = face->glyph->bitmap_top;
		Characters[c].Advance = face->glyph->advance.x;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &TEXT_VAO);
	glGenBuffers(1, &TEXT_VBO);
	glBindVertexArray(TEXT_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, TEXT_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	hgeUseShader(hgeResourcesQueryShader("sprite_shader"));
}

void televoidTextRenderSimple(const char* text, bool centered, hge_vec3 position, float scale, hge_vec4 color)
{
	// Clear the colorbuffer
	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	televoidTextRender(hgeResourcesQueryShader("text"), centered, text, position, 0, 0, scale, color);
}

void televoidTextRender(hge_shader shader, bool centered, const char* text, hge_vec3 position, GLfloat x, GLfloat y, GLfloat scale, hge_vec4 color)
{
	// Activate corresponding render state
  hgeUseShader(shader);
	glUniform4f(glGetUniformLocation(shader.id, "textColor"), color.x, color.y, color.z, color.w);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(TEXT_VAO);

	// Change model
	hge_mat4 model = hgeMathMat4(1.0f);
  model = hgeMathMat4Translate(model, position);  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)

	/*model = glm::translate(model, glm::vec3(0.5f * transform->GetScale()->GetX(), 0.5f * transform->GetScale()->GetY(), 0.5f * transform->GetScale()->GetZ())); // Move origin of rotation to center of quad

	glm::quat q = glm::quat(-transform->GetRot()->GetZ(), -transform->GetRot()->GetY(), -transform->GetRot()->GetX(), transform->GetRot()->GetW());
	model *= glm::toMat4(q);

	model = glm::translate(model, glm::vec3(-0.5f * transform->GetScale()->GetX(), -0.5f * transform->GetScale()->GetY(),-0.5f * transform->GetScale()->GetZ())); // Move origin back

	model = glm::scale(model, glm::vec3(transform->GetScale()->GetX(), transform->GetScale()->GetY(), transform->GetScale()->GetZ()));*/ // Last scale

  hgeShaderSetMatrix4(shader, "model", model);

	float bx = x;

	// Iterate through all characters
	//std::string::const_iterator c;
  int i;
	for (i = 0; i < strlen(text); i++)
	{
    char c = text[i];
		Character ch = Characters[c];

		if (c == '\n')
		{
			x = bx;
			y -= 50*scale;
		}
		else
		{

			GLfloat xpos = x + ch.Bearing.x * scale;
			GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

			GLfloat w = ch.Size.x * scale;
			GLfloat h = ch.Size.y * scale;
			if(!centered)
			{
				// Update VBO for each character
				GLfloat vertices[6][4] = {
					{ xpos, ypos + h, 0.0, 0.0 },
					{ xpos, ypos, 0.0, 1.0 },
					{ xpos + w, ypos, 1.0, 1.0 },

					{ xpos, ypos + h, 0.0, 0.0 },
					{ xpos + w, ypos, 1.0, 1.0 },
					{ xpos + w, ypos + h, 1.0, 0.0 }
				};
				// Render glyph texture over quad
				glBindTexture(GL_TEXTURE_2D, ch.TextureID);
				// Update content of VBO memory
				glBindBuffer(GL_ARRAY_BUFFER, TEXT_VBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				// Render quad
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
		}
	}

	/*if(centered)
	{
		float xs = x;
		x = bx;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = Characters[*c];
			if (*c == '\n')
			{
				x = bx;
				y -= 50*scale;
			}
			else
			{
				GLfloat xpos = x-(fabs(bx-xs))/2 + ch.Bearing.x * scale;
				GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

				GLfloat w = ch.Size.x * scale;
				GLfloat h = ch.Size.y * scale;
				// Update VBO for each character
				GLfloat vertices[6][4] = {
					{ xpos, ypos + h, 0.0, 0.0 },
					{ xpos, ypos, 0.0, 1.0 },
					{ xpos + w, ypos, 1.0, 1.0 },

					{ xpos, ypos + h, 0.0, 0.0 },
					{ xpos + w, ypos, 1.0, 1.0 },
					{ xpos + w, ypos + h, 1.0, 0.0 }
				};


				// Render glyph texture over quad
				glBindTexture(GL_TEXTURE_2D, ch.TextureID);
				// Update content of VBO memory
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				// Render quad
				glDrawArrays(GL_TRIANGLES, 0, 6);
				// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
				x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
			}
		}
	}*/

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
