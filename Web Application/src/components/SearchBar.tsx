interface Props {
    value: string;
    onChange: (val: string) => void;
    onSearch: () => void;
  }
  
  export default function SearchBar({ value, onChange, onSearch }: Props) {
    return (
      <div className="flex items-center gap-2 w-full max-w-xl">
        <input
          type="text"
          placeholder="Search for content or path..."
          className="flex-grow p-2 border rounded shadow-sm focus:outline-none focus:ring focus:border-blue-300"
          value={value}
          onChange={(e) => onChange(e.target.value)}
        />
        <button
          className="bg-blue-600 text-white px-4 py-2 rounded hover:bg-blue-700"
          onClick={onSearch}
        >
          Search
        </button>
      </div>
    );
  }
  