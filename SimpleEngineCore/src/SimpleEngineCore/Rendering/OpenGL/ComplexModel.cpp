#include "ComplexModel.hpp"

namespace SimpleEngine
{
	ComplexModel::ComplexModel(const std::vector<ModelData>& model_paths)
	{
		for (auto& e : model_paths)
		{
			models.push_back(std::make_unique<Model>(e.model_path.c_str(), e.texture_path.c_str()));
		}
	}

	void ComplexModel::Render()
	{
		for (auto& e : models)
		{
			e->render();
		}
	}

	void ComplexModel::set_material(const Material& new_material, size_t number)
	{
		models[number]->set_material(new_material);
	}

	const Material& ComplexModel::get_material(size_t number) const noexcept
	{
		return models[number]->get_material();
	}

	void ComplexModel::set_scale(glm::vec3 new_scale)
	{
		for (auto& e : models)
		{
			e->set_scale(new_scale);
		}
		scale = new_scale;
	}

	void ComplexModel::set_location(glm::vec3 new_location)
	{
		for (auto& e : models)
		{
			e->set_location(new_location);
		}
		location = new_location;
	}

	void ComplexModel::set_rotation(glm::vec3 new_rotation)
	{
		for (auto& e : models)
		{
			e->set_rotation(new_rotation);
		}
		rotation = new_rotation;
	}

	void ComplexModel::update_camera(const Camera& camera, const std::string& view_name, const std::string& pos_name) const
	{
		for (auto& e : models)
		{
			camera.set_matrix(e->get_shader_program(), view_name.c_str());
			camera.set_position(e->get_shader_program(), pos_name.c_str());
		}
	}

	void ComplexModel::update_light(const Light& light) const
	{
		for (auto& e : models)
		{
			light.update_shader(e->get_shader_program());
		}
	}

}