# Laboratorio de Computación Gráfica e Interacción Humano-Computadora

**Rodrigo Jardón Marín** · No. de cuenta 321240693 · Grupo de laboratorio 09 · Semestre 2027-1

Prácticas de OpenGL en C++ (GLFW, GLEW, glm, assimp, SOIL2 y stb_image).

## Cómo probar una práctica

1. Abrir **`CompuGrafica-Lab.sln`** con **Visual Studio 2022** (o más reciente).
2. Arriba, dejar la configuración en **Debug** y la plataforma en **x86**.
3. En el *Explorador de soluciones*, clic derecho sobre la práctica →
   **Establecer como proyecto de inicio**.
4. Presionar **F5**.

No hace falta configurar nada: las librerías están en `External Libraries/` y
cada proyecto las encuentra con rutas relativas (archivo `Comun.props`).

## Prácticas

| Carpeta | Tema | Controles |
|---|---|---|
| `P00_ConfiguracionBase` | Configuración base: ventana y primer triángulo | — |
| `P01_Dibujo2D` | Dibujo de primitivas en 2D | — |
| `P02_ProyeccionesTransformaciones` | Proyecciones y transformaciones básicas (4 cubos) | W/S, A/D, PgUp/PgDn mueven la cámara; flechas la rotan |
| `P03_ModeladoGeometrico` | Modelado geométrico (mesa) | W/S, A/D, PgUp/PgDn mueven la cámara; flechas y Q/E la rotan |
| `P04_ModeladoJerarquico` | Modelado jerárquico (brazo con 3 dedos) | Cámara: W/S, A/D, ↑/↓, ←/→ (girar) · Hombro R/F · Codo T/G · Muñeca Y/H · Dedo 1 U/J, I/K · Dedo 2 O/L, P/M · Dedo 3 7/8, 9/0 |
| `P05_CargaModelos` | Carga de modelos y cámara sintética (perros y gatos) | W/A/S/D o flechas para moverse; mouse para girar |
| `P06_Texturizado` | Texturizado (recortes y mosaico con transparencia) | W/A/S/D o flechas para moverse; mouse para girar |
| `P07_MaterialesIluminacion` | Materiales e iluminación (2 luces) | W/A/S/D o flechas · O/L mueven la luz 1 · I/K mueven la luz 2 |
| `P08_FuentesDeLuz` | Fuentes de luz (4 luces de colores, día y noche) | W/A/S/D o flechas · ver comentarios del código para luces y día/noche |
| `P09_MaquinaEstados` | Animación por máquina de estados (perro por partes y pelota) | W/A/S/D o flechas · ver comentarios del código para la animación |

## Notas técnicas

- Conjunto de herramientas: **v143 (Visual Studio 2022)**, Windows SDK 10.0 (más reciente instalado).
- Plataforma: **x86 (Win32)**, porque las librerías incluidas son de 32 bits.
- Cada práctica tiene sus propios shaders (`Shader/`), modelos (`Models/`) e imágenes.
- El `.exe` se genera en `<práctica>/bin/Debug/` y las DLL se copian ahí al compilar.
