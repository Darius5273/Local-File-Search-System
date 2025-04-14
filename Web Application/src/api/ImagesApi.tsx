import axios from 'axios';

const axiosInstance = axios.create({
  baseURL: 'http://localhost:8080', 
  timeout: 0, 
  headers: { 'Content-Type': 'application/json' },
});

export async function fetchImage(path: string): Promise<string> {
  try {
    const response = await axiosInstance.get('/image', {
      params: { path }, 
      responseType: 'blob', 
    });

    const imageUrl = URL.createObjectURL(response.data);

    return imageUrl; 
  } catch (error) {
    if (axios.isAxiosError(error) && error.response) {
      const errorMessage = error.response.data.error || 'Image request failed';
      throw new Error(errorMessage);
    } else {
      throw new Error('Image request failed');
    }
  }
}
