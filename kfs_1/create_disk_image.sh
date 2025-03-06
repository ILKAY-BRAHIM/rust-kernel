#!/bin/bash

set -e

DISK_IMAGE="myos.img"
KERNEL_BIN="kernel.bin"

# === Config ===
SECTOR_SIZE=512
TOTAL_SECTORS=$((4 * 1024 * 1024 / $SECTOR_SIZE))  # 64 MB

# === Create empty disk image ===
echo "[+] Creating empty disk image ($DISK_IMAGE)"
dd if=/dev/zero of=$DISK_IMAGE bs=$SECTOR_SIZE count=$TOTAL_SECTORS

# === Partition the disk image ===
echo "[+] Partitioning disk image with parted..."
sudo parted -s $DISK_IMAGE mklabel msdos
sudo parted -s $DISK_IMAGE mkpart primary ext2 2048s 100%

# === Setup loop device ===
LOOP_DEVICE=$(sudo losetup -fP --show $DISK_IMAGE)
echo "[+] Attached $DISK_IMAGE to $LOOP_DEVICE"

# === Verify partition ===
PARTITION="${LOOP_DEVICE}p1"
if [ ! -e "$PARTITION" ]; then
    echo "[-] ERROR: Partition $PARTITION not found!"
    sudo losetup -d $LOOP_DEVICE
    exit 1
fi
echo "[+] Partition found: $PARTITION"

# === Format partition ===
echo "[+] Formatting $PARTITION as ext2"
sudo mkfs.ext2 -F -m 0 $PARTITION

# === Create temporary mount point ===
MOUNT_DIR=$(mktemp -d)
echo "[+] Created temporary mount point: $MOUNT_DIR"

# === Mount partition ===
sudo mount $PARTITION $MOUNT_DIR

# === Install GRUB ===
echo "[+] Setting up GRUB"
sudo mkdir -p $MOUNT_DIR/boot/grub
sudo cp $KERNEL_BIN $MOUNT_DIR/boot/

cat << EOF | sudo tee $MOUNT_DIR/boot/grub/grub.cfg
menuentry "My Kernel" {
    multiboot /boot/kernel.bin
    boot
}
EOF

# === Install GRUB ===
echo "[+] Installing GRUB"
sudo grub-install \
    --target=i386-pc \
    --boot-directory=$MOUNT_DIR/boot \
    --no-floppy \
    --modules="multiboot ext2 biosdisk part_msdos" \
    --compress=no \
    --fonts=no \
    --themes=no \
    --locales="" \
    $LOOP_DEVICE

# === Cleanup ===
echo "[+] Unmounting and detaching loop device"

# 1. Lazy unmount (detach filesystem immediately)
sudo umount -l $MOUNT_DIR 2>/dev/null || true

# 2. Force remove temporary directory
sudo rm -rf $MOUNT_DIR

# 3. Detach loop device
sudo losetup -d $LOOP_DEVICE
chmod 644 myos.img
sudo chown $USER:$USER myos.img


echo "✅ Disk image created successfully: $DISK_IMAGE"