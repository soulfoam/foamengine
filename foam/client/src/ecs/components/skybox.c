#include <foam/client/ecs/components/skybox.h>
#include <foam/core/data/stb_image.h>
#include <foam/core/ext/logger.h>
#include <foam/client/gfx/renderer.h>

FE_RETURN_CODE
fe_skybox_make(fe_skybox_t *skybox, u32 texture_count, const char **textures)
{
    FE_RETURN_CODE rc = FE_OK;

	glGenTextures(1, &skybox->id);

    fe_skybox_bind(skybox);

    for (u32 i = 0; i < texture_count; i++)
    { 
        i32 width = 0; i32 height = 0; i32 comp = 0;
        
        u8 *img = NULL;
        
        img = stbi_load(textures[i], &width, &height, &comp, STBI_rgb_alpha);
        if (!img)
        {
            rc = FE_ERR_PATH_NOT_FOUND;
            fe_log_return_code(rc, "Couldn't load texture: %s", textures[i]);
            fe_skybox_kill(skybox);
            return rc;
        }
 
	    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);

        stbi_image_free(img);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    fe_skybox_unbind();

    GLfloat vertices[] = 
    {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

	glGenVertexArrays(1, &skybox->vao);
    glGenBuffers(1, &skybox->vbo);

    glBindVertexArray(skybox->vao);

    glBindBuffer(GL_ARRAY_BUFFER, skybox->vbo);
    glBufferData(
			GL_ARRAY_BUFFER, 
			FE_VERTS_IN_BYTES(36), 
			vertices, 
			GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(0);


	glBindVertexArray(0);

    return rc;
}

void
fe_skybox_kill(fe_skybox_t *skybox)
{
	glDeleteTextures(1, &skybox->id);
	glDeleteVertexArrays(1, &skybox->vao);
	glDeleteBuffers(1, &skybox->vbo);
}

void
fe_skybox_bind(const fe_skybox_t *skybox)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->id);
}

void
fe_skybox_unbind(void)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
