#pragma once

#include "iostream"

#include "Core/Log.h"
#include "Core/SaveSystem.h"
#include "Core/Model.h"
#include "Core/Shader.h"

#include "Components/Components.h"
#include "Components/Transform.h"

namespace Nifty
{

	class Entity : public SaveItem
	{
	private:
		std::vector<Component*> m_Components;

	public:
		Transform transform;
		std::string tag;

	public:

		Entity(Transform transform = Transform(), std::string name = "New Entity", unsigned int id = 0); // create 3D entity
		~Entity() {}

		std::vector<Component*> GetAllComponents() const { return m_Components; }

		void Draw(Shader& shader, glm::mat4& matrix);
		std::string Save();

		template<typename T>
		T* GetComponent()
		{
			for (unsigned int i = 0; i < m_Components.size(); ++i)
			{
				if (typeid(T) == typeid(*m_Components[i]))
				{
					return (T*)m_Components[i];
				}
			}
			return nullptr;
		}

		template<typename T>
		void AddComponent(T* new_component)
		{
			for (unsigned int i = 0; i < m_Components.size(); ++i)
			{
				if (typeid(T) == typeid(*m_Components[i])) {
					Log::Error("Entity already has " + m_Components[i]->name + " component!");
					return;
				}
			}
			m_Components.push_back(new_component);
		}

		template<typename T>
		void RemoveComponent()
		{
			for (Component* component : m_Components)
			{
				if (typeid(T) == typeid(*component)) {
					if (!component) return; // check if component still exists
					auto itr = std::find(m_Components.begin(), m_Components.end(), component); // find in vector
					if (itr == m_Components.end()) return; // check if component is in vector
					m_Components.erase(itr); // remove from vector
				}
			}
		}

		static Entity* FindWithName(const std::string& name, std::vector<Entity*>& entities);
	};
}

