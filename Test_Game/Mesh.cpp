#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	: m_vertices(vertices), m_indices(indices), m_textures(textures) {

	std::cout << vertices.size() << " " << indices.size() << textures.size() << std::endl;

	setupMesh();
}

Mesh::~Mesh() {

}

void Mesh::setupMesh() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex),&m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),	(void*)offsetof(Vertex, normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),	(void*)offsetof(Vertex, texcoord));
	glBindVertexArray(0);
}

void Mesh::Draw(Shader& shader)
{
	//std::cout << m_vertices.size() << " " << &shader <<  std::endl;
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // activate texture unit first
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = m_textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		shader.setF1(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i].ID);
	}
	glActiveTexture(GL_TEXTURE0);
	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

unsigned int Mesh::GetVAO() {
	return VAO;
}
