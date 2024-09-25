
# Tutorial: Configuración de Clúster con OpenMPI

Este tutorial describe cómo configurar rápidamente un clúster con OpenMPI para ejecutar aplicaciones en paralelo y distribuidas.

## Paso 1: Preparar todos los nodos (Principal y Secundarios)

1. **Actualizar e instalar dependencias básicas**:
   ```bash
   sudo apt update && sudo apt-get upgrade
   sudo apt install build-essential net-tools openssh-server openmpi-bin libopenmpi-dev
   ```

2. **Configurar SSH (en todos los nodos)**:

   - Genera claves SSH:
     ```bash
     ssh-keygen -t rsa -N "" -f ~/.ssh/id_rsa
     ```

   - **En el nodo principal**, comparte la clave pública con los secundarios:
     ```bash
     ssh-copy-id user@sec1_ip
     ssh-copy-id user@sec2_ip
     ```

## Paso 2: Configurar NFS (para compartir archivos)

1. **Nodo principal**:

   - Instala el servidor NFS:
     ```bash
     sudo apt install nfs-kernel-server
     ```
   - Crea y configura la carpeta compartida:
     ```bash
     sudo mkdir -p /home/usuario/sharedMPI
     sudo chown nobody:nogroup /home/usuario/sharedMPI
     sudo chmod 777 /home/usuario/sharedMPI
     ```
   - Edita `/etc/exports`:
     ```bash
     sudo vim /etc/exports
     /home/usuario/sharedMPI sec1_ip(rw,sync,no_subtree_check) sec2_ip(rw,sync,no_subtree_check)
     ```
   - Aplica los cambios:
     ```bash
     sudo exportfs -a
     sudo systemctl restart nfs-kernel-server
     ```

2. **Nodos secundarios**:

   - Instala cliente NFS:
     ```bash
     sudo apt install nfs-common
     ```
   - Monta la carpeta compartida:
     ```bash
     sudo mkdir -p /home/usuario/sharedMPI
     sudo mount main_ip:/home/usuario/sharedMPI /home/usuario/sharedMPI
     ```

## Paso 3: Probar la instalación de OpenMPI

1. **Compilar y correr un programa MPI**:
   
   - Compila el código en el nodo principal:
     ```bash
     mpicc hola_mundo.c -o hola_mundo
     ```

   - Crea un archivo `hostfile`:
     ```bash
     vim hostfile
     # Agregar:
     sec1_ip
     sec2_ip
     ```

   - Ejecuta en paralelo:
     ```bash
     mpirun --hostfile hostfile -np 4 ./hola_mundo
     ```


## Contribuidores
  - Anthony Mamani 
  - Justo Perez
  - Abimael Ruiz
  - Mariana Caceres
  - Emmanuel Galdos