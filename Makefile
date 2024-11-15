


# Основной Makefile для управления проектами
.PHONY: all clean 2DGameEngine 2DPhysicEngine

# Цель по умолчанию
all: 2DGameEngine 2DPhysicEngine

# Сборка 2DGameEngine
2DGameEngine:
	@echo "Building 2DGameEngine..."
	$(MAKE) -C 2DGameEngine

# Сборка 2DPhysicEngine
2DPhysicEngine:
	@echo "Building 2DPhysicEngine..."
	$(MAKE) -C 2DPhysicEngine

# Очистка всех проектов
clean:
	@echo "Cleaning all projects..."
	$(MAKE) -C 2DGameEngine clean
	$(MAKE) -C 2DPhysicEngine clean


