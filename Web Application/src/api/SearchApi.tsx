import axios from 'axios';
import { SearchResponse } from '../interfaces/SearchResponse';


const axiosInstance = axios.create({
    baseURL: 'http://localhost:8080',
    timeout: 10000, 
    headers: { 'Content-Type': 'application/json' },
  });

export async function searchFiles(query: string): Promise<SearchResponse> {
  try {
    const response = await axiosInstance.post('/search', { query }, {
      headers: {
        'Content-Type': 'application/json',
      },
    });

    return response.data as SearchResponse;
  } catch (error) {
    if (axios.isAxiosError(error) && error.response) {
      const errorMessage = error.response.data.error || 'Search request failed';
      throw new Error(errorMessage);
    } else {
      throw new Error('Search request failed');
    }
  }
}
