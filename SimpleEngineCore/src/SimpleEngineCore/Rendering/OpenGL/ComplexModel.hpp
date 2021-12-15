#ifndef COMPLEX_MODEL_HPP
#define COMPLEX_MODEL_HPP
#include <vector>
#include <SimpleEngineCore/Rendering/OpenGL/Model.hpp>
#include <SimpleEngineCore/Rendering/OpenGL/Camera.hpp>
#include <SimpleEngineCore/Rendering/OpenGL/Light.hpp>

namespace SimpleEngine
{
struct ModelData
{
	using string = std::string;
	ModelData(string model, string texture)
		: model_path(std::move(model)), texture_path(std::move(texture)) {};
	string model_path;
	string texture_path;
};

class ComplexModel
{
public:
	ComplexModel(const std::vector<ModelData>& model_paths);
	void Render();

	glm::vec3 get_scale() const noexcept { return scale; }
	glm::vec3 get_location() const noexcept { return location; }
	glm::vec3 get_rotation() const noexcept { return rotation; }

	void set_scale(glm::vec3 new_scale);
	void set_location(glm::vec3 new_location);
	void set_rotation(glm::vec3 new_rotation);

	void UpdateCamera(const Camera& camera, const std::string& view_name, const std::string& pos_name) const;
	void UpdateLight(const Light& light) const;
	const ShaderProgram& get_shader_program() const { return models[0]->get_shader_program(); }
private:
	std::vector<std::unique_ptr<Model>> models;
	glm::vec3 scale{ 1.f, 1.f, 1.f };
	glm::vec3 rotation{ 0.f, 0.f, 0.f };
	glm::vec3 location{ 0.f, 0.f, 0.f };
};

}

#endif //COMPLEX_MODEL_HPP