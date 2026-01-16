from PIL import Image
import sys

# 兼容不同版本的Pillow
try:
    # Pillow >= 9.0
    RESAMPLE_FILTER = Image.Resampling.LANCZOS
except AttributeError:
    # Pillow < 9.0
    RESAMPLE_FILTER = Image.LANCZOS

def image_to_matrix(image_path, target_height=8, invert=False, threshold=128):
    try:
        # 1. 打开图片
        img = Image.open(image_path)
        w, h = img.size

        # 2. 计算新宽度，保持纵横比 (Aspect Ratio)
        # 公式：新宽度 = 原宽度 * (目标高度 / 原高度)
        new_width = int(w * target_height / h)
        if new_width == 0: new_width = 1

        print(f"原始尺寸: {w}x{h} -> 缩放后: {new_width}x{target_height}")

        # 3. 缩放图片 (使用 ANTIALIAS/LANCZOS 平滑缩放，防止丢失细节)
        img = img.resize((new_width, target_height), RESAMPLE_FILTER)

        # 4. 转为灰度再转为二值化 (黑白)
        img = img.convert('L') # 转灰度
        # 自定义二值化：大于阈值为白(1)，否则为黑(0)
        # 如果是OLED屏幕，通常 1=亮，0=灭。
        # 如果你的图片是白底黑字，你可能需要反转颜色 (invert=True)
        pixel_data = img.load()
        
        output_bytes = []

        # 5. 垂直扫描取模 (Column by Column)
        for x in range(new_width):
            col_byte = 0
            for y in range(target_height):
                # 获取像素值 (0-255)
                pixel_val = pixel_data[x, y]
                
                # 判断亮灭
                is_pixel_on = pixel_val > threshold
                
                if invert:
                    is_pixel_on = not is_pixel_on

                # 如果点亮，将对应的位(Bit)置1
                # 这里默认：LSB(低位) 在上，MSB(高位) 在下 (D0对应y=0)
                if is_pixel_on:
                    col_byte |= (1 << (7-y))
            
            output_bytes.append(col_byte)

        return output_bytes

    except Exception as e:
        print(f"错误: {e}")
        return []


def visualize_matrix(byte_array, height=8):
    """
    可视化LED点阵数据
    :param byte_array: 字节数组，每个字节代表一列
    :param height: LED高度（默认8）
    """
    if not byte_array:
        print("空数组，无法可视化")
        return
    
    width = len(byte_array)
    
    print(f"\n{'='*60}")
    print(f"LED点阵可视化 ({width}列 × {height}行)")
    print(f"{'='*60}")
    
    # 打印列号
    print("    列:", end="")
    for x in range(width):
        print(f" {x:2d}", end="")
    print()
    print("    " + "-" * (width * 3 + 5))
    
    # 逐行打印
    for y in range(height):
        print(f"行{y}(D{y}):", end="")
        for x in range(width):
            byte_val = byte_array[x]
            # 检查第y位是否为1
            if byte_val & (1 << y):
                print("  █", end="")  # 亮
            else:
                print("  ·", end="")  # 灭
        print()
    
    print(f"{'='*60}\n")
    
    # 同时打印二进制表示
    print("二进制表示（每列）:")
    for i, byte_val in enumerate(byte_array):
        binary = format(byte_val, '08b')
        print(f"  列{i:2d}: 0x{byte_val:02X} = 0b{binary} = {byte_val:3d}")


# 使用示例
if __name__ == "__main__":
    # 在这里修改你的图片路径
    img_path = "/home/hysin/Downloads/test2.png" 
    
    # 获取数组
    result = image_to_matrix(img_path, target_height=8, invert=True)
    
    # 可视化显示
    visualize_matrix(result)
    
    # 打印为 C 语言数组格式
    hex_str = ", ".join([f"0x{b:02X}" for b in result])
    print(f"C语言数组 ({len(result)} 字节):")
    print(f"{{ {hex_str} }};")
