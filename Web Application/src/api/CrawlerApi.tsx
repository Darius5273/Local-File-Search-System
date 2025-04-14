import axios from 'axios';
import { CrawlRequest } from "../interfaces/CrawlRequest";


const axiosInstance = axios.create({
  baseURL: 'http://localhost:8080',
  timeout: 0, 
  headers: { 'Content-Type': 'application/json' },
});


export async function crawlDirectory(config: CrawlRequest): Promise<{ message: string }> {
  try {
    console.log(config)
    const response = await axiosInstance.post('/crawl', config, {
      headers: {
        'Content-Type': 'application/json',
      },
    });

    return response.data;
  } catch (error) {
    if (axios.isAxiosError(error) && error.response) {
      const errorMessage = error.response.data.error || 'Crawler request failed';
      throw new Error(errorMessage);
    } else {
      throw new Error('Crawler request failed');
    }
  }
}